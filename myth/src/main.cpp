#include <SDL.h>
#include <SOIL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <list>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <gl_loader/gl_loader.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/foreach.hpp>

#include <debug.h>
#include <shader.h>
#include <buffers.h>
#include <texture.h>
#include <camera.h>
#include <sampler.h>
#include <textureunit.h>
#include <renderstate.h>
#include <clearstate.h>

//-----------------------------------------------------

struct Vertex
{
  glm::vec3 position;
  glm::vec2 textureCoord;
};

struct GlobalUniforms
{
  glm::vec4 CameraPos;
  glm::mat4 WorldMatrix;
  glm::mat4 ViewMatrix;
  glm::mat4 ProjectionMatrix;
  glm::mat4 InverseWorldMatrix;
  glm::mat4 InverseViewMatrix;
  glm::mat4 WorldViewMatrix;
  glm::mat4 ViewProjectionMatrix;
  glm::mat4 WorldViewProjectionMatrix;
};

struct ModelAsset
{
  GLuint vao;
  GLenum drawType;
  GLint drawFirstVertex;
  GLint drawVertexCount;
  boost::shared_ptr<Shader> shader;
  boost::shared_ptr<Sampler2D> sampler;
  boost::shared_ptr<Texture2D> texture;
  boost::shared_ptr<VertexBuffer> vertexBuffer;
};

struct ModelInstance
{
  glm::mat4 transform;
  boost::shared_ptr<ModelAsset> asset;
};

// Maintains the moment-by-moment of the OpenGL context.
// This is used to prevent redundant calls to change state.
struct Device
{
  ClearState clearState;
  RenderState renderState;
};

//-----------------------------------------------------

FILE* debug::logFile;
static SDL_Window* mainWindow;
static SDL_GLContext glContext;

static Device device;

static boost::shared_ptr<UniformBuffer> uniformBuffer;
static GlobalUniforms                   globalUniforms;

static boost::shared_ptr<ModelAsset> woodenCrate;
static std::list<boost::shared_ptr<ModelInstance>> instances;

static boost::shared_ptr<Camera> camera;

static float rotation = 0.0f;

//-----------------------------------------------------

static void Init(const std::string& title, int width, int height, bool fullScreen);
static void CreateObject();
static void CreateInstances();
static void Update(unsigned int elapsedMS);
static void Render();
static void ApplyShader(boost::shared_ptr<Shader> shader, RenderState& state);
static void ApplyTextureUnits(boost::shared_ptr<Sampler2D> samplers[], RenderState& state);
static void ForceClearState(const ClearState& state);
static void ForceRenderState(const RenderState& state);
static void ApplyShader(boost::shared_ptr<Shader> shader, RenderState& state);
static void ApplyTextureUnits(TextureUnit updated[], TextureUnit current[]);
static void ApplyVAO(GLuint updated, RenderState& state);

//-----------------------------------------------------

int main(int argc, char* argv[])
{
  Init("Myth", 1280, 720, false);
  
  uniformBuffer = boost::make_shared<UniformBuffer>(sizeof(GlobalUniforms), 0);

  const float aspectRatio((float)1280/(float)720);
  camera = boost::make_shared<Camera>(45.0f, aspectRatio, 0.1f, 100.0f);
  camera->Position = glm::vec3(-5,2,15);

  CreateObject();
  CreateInstances();

  bool quit = false;
  unsigned int prevTime = 0;

  while (!quit)
  {
    {
      SDL_Event event;
      while (SDL_PollEvent(&event))
      {
        switch (event.type)
        {
        case SDL_QUIT: quit = true; break;
        case SDL_KEYDOWN:
          {
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) { quit = true; }
          }
          break;
        default: break;
        }
      }
    }

    unsigned int now = SDL_GetTicks();
    if (prevTime == 0) { prevTime = now; }
    unsigned int elapsedMS = now - prevTime;
    prevTime = now;

    Update(elapsedMS);
    Render();
  }

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(mainWindow);

  return 0;
}

//-----------------------------------------------------
static void Init(const std::string& title, int width, int height, bool fullScreen)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	atexit(SDL_Quit);

	debug::logFile = fopen("stderr.txt", "wb");

  static const struct SDLAttribute
  {
    SDL_GLattr  attr;
    int         value;
  } sdlAttributes[] =
  {
    { SDL_GL_DOUBLEBUFFER,       1 },
    { SDL_GL_RED_SIZE,           8 },
    { SDL_GL_GREEN_SIZE,         8 },
    { SDL_GL_BLUE_SIZE,          8 },
    { SDL_GL_ALPHA_SIZE,         8 },
    { SDL_GL_BUFFER_SIZE,        24 },
    { SDL_GL_MULTISAMPLEBUFFERS, 1 },
    { SDL_GL_MULTISAMPLESAMPLES, 4 }
  };
  const int NumAttributes = sizeof(sdlAttributes)/sizeof(sdlAttributes[0]);
  for (int i = 0; i < NumAttributes; ++i)
  {
    SDL_GL_SetAttribute(sdlAttributes[i].attr, sdlAttributes[i].value);
  }

  if (fullScreen)
  {
    mainWindow = SDL_CreateWindow(
      title.c_str(),
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      0, 0, // width & height are ignored in full screen mode
      SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
  }
  else
  {
    mainWindow = SDL_CreateWindow(
      title.c_str(),
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      width, height,
      SDL_WINDOW_OPENGL);
  }

	glContext = SDL_GL_CreateContext(mainWindow);

	ogl_LoadFunctions();
  LOG("GL v%d.%d\n", ogl_GetMajorVersion(), ogl_GetMinorVersion());
  LOG("GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
  LOG("Vendor: %s\n", glGetString(GL_VENDOR));
  LOG("Renderer: %s\n", glGetString(GL_RENDERER));

  ForceClearState(device.clearState);
  ForceRenderState(device.renderState);

  SDL_SetRelativeMouseMode(SDL_TRUE);
}

//-----------------------------------------------------
static void CreateObject()
{
  static Vertex vertices[] =
  {
    // bottom
    { glm::vec3(-1.0f,-1.0f,-1.0f), glm::vec2(0.0f, 0.0f) },
    { glm::vec3( 1.0f,-1.0f,-1.0f), glm::vec2(1.0f, 0.0f) },
    { glm::vec3(-1.0f,-1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },
    { glm::vec3( 1.0f,-1.0f,-1.0f), glm::vec2(1.0f, 0.0f) },
    { glm::vec3( 1.0f,-1.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
    { glm::vec3(-1.0f,-1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },

    // top
    { glm::vec3(-1.0f, 1.0f,-1.0f), glm::vec2(0.0f, 0.0f) },
    { glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },
    { glm::vec3( 1.0f, 1.0f,-1.0f), glm::vec2(1.0f, 0.0f) },
    { glm::vec3( 1.0f, 1.0f,-1.0f), glm::vec2(1.0f, 0.0f) },
    { glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },
    { glm::vec3( 1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f) },

    // front
    { glm::vec3(-1.0f,-1.0f, 1.0f), glm::vec2(1.0f, 0.0f) },
    { glm::vec3( 1.0f,-1.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
    { glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
    { glm::vec3( 1.0f,-1.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
    { glm::vec3( 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },
    { glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f) },

    // back
    { glm::vec3(-1.0f,-1.0f,-1.0f), glm::vec2(0.0f, 0.0f) },
    { glm::vec3(-1.0f, 1.0f,-1.0f), glm::vec2(0.0f, 1.0f) },
    { glm::vec3( 1.0f,-1.0f,-1.0f), glm::vec2(1.0f, 0.0f) },
    { glm::vec3( 1.0f,-1.0f,-1.0f), glm::vec2(1.0f, 0.0f) },
    { glm::vec3(-1.0f, 1.0f,-1.0f), glm::vec2(0.0f, 1.0f) },
    { glm::vec3( 1.0f, 1.0f,-1.0f), glm::vec2(1.0f, 1.0f) },

    // left
    { glm::vec3(-1.0f,-1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },
    { glm::vec3(-1.0f, 1.0f,-1.0f), glm::vec2(1.0f, 0.0f) },
    { glm::vec3(-1.0f,-1.0f,-1.0f), glm::vec2(0.0f, 0.0f) },
    { glm::vec3(-1.0f,-1.0f, 1.0f), glm::vec2(0.0f, 1.0f) },
    { glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
    { glm::vec3(-1.0f, 1.0f,-1.0f), glm::vec2(1.0f, 0.0f) },

    // right
    { glm::vec3( 1.0f,-1.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
    { glm::vec3( 1.0f,-1.0f,-1.0f), glm::vec2(1.0f, 0.0f) },
    { glm::vec3( 1.0f, 1.0f,-1.0f), glm::vec2(0.0f, 0.0f) },
    { glm::vec3( 1.0f,-1.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
    { glm::vec3( 1.0f, 1.0f,-1.0f), glm::vec2(0.0f, 0.0f) },
    { glm::vec3( 1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f) }
  };

  woodenCrate = boost::make_shared<ModelAsset>();

  woodenCrate->drawType = GL_TRIANGLES;
  woodenCrate->drawFirstVertex = 0;
  woodenCrate->drawVertexCount = sizeof(vertices) / sizeof(vertices[0]);

  glGenVertexArrays(1, &woodenCrate->vao);
  glBindVertexArray(woodenCrate->vao);

  woodenCrate->vertexBuffer = boost::make_shared<VertexBuffer>(sizeof(vertices), GL_STATIC_DRAW);
  woodenCrate->vertexBuffer->Enable();
  woodenCrate->vertexBuffer->SetData(vertices, sizeof(vertices), 0);

  woodenCrate->shader = boost::make_shared<Shader>("shaders/textured");

  woodenCrate->sampler = boost::make_shared<Sampler2D>();

  woodenCrate->texture = boost::make_shared<Texture2D>("images/wooden-crate.jpg");
  woodenCrate->texture->Load();

  glEnableVertexAttribArray(woodenCrate->shader->GetAttributeIndex("Position"));
  glVertexAttribPointer(woodenCrate->shader->GetAttributeIndex("Position"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));

  glEnableVertexAttribArray(woodenCrate->shader->GetAttributeIndex("TextureCoord"));
  glVertexAttribPointer(woodenCrate->shader->GetAttributeIndex("TextureCoord"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, textureCoord));

  VertexBuffer::Disable();
  glBindVertexArray(0);
}

//-----------------------------------------------------
static void Update(unsigned int elapsedMS)
{
  const float elapsedSeconds = (float)elapsedMS / 1000.0f;

  // keyboard input...
  {
    const float unitsPerSecond = 2.0f;
    const Uint8* pressedKeys = SDL_GetKeyboardState(NULL);
    if (pressedKeys[SDL_SCANCODE_W]) { camera->Position += (camera->Forward * unitsPerSecond * elapsedSeconds); }
    if (pressedKeys[SDL_SCANCODE_S]) { camera->Position += (-camera->Forward * unitsPerSecond * elapsedSeconds); }
    if (pressedKeys[SDL_SCANCODE_A]) { camera->Position += (-camera->Right * unitsPerSecond * elapsedSeconds); }
    if (pressedKeys[SDL_SCANCODE_D]) { camera->Position += (camera->Right * unitsPerSecond * elapsedSeconds); }
    if (pressedKeys[SDL_SCANCODE_X]) { camera->Position += (glm::vec3(0,1,0) * unitsPerSecond * elapsedSeconds); }
    if (pressedKeys[SDL_SCANCODE_Z]) { camera->Position += (glm::vec3(0,-1,0) * unitsPerSecond * elapsedSeconds); }
    if (pressedKeys[SDL_SCANCODE_LEFT]) { camera->Yaw -= 0.1f; }
    if (pressedKeys[SDL_SCANCODE_RIGHT]) { camera->Yaw += 0.1f; }
    if (pressedKeys[SDL_SCANCODE_UP]) { camera->Pitch -= 0.1f; }
    if (pressedKeys[SDL_SCANCODE_DOWN]) { camera->Pitch += 0.1f; }
  }

  // mouse input...
  {
    const float sensitivity = 0.1f;
    glm::ivec2 position;
    const Uint32 buttons = SDL_GetRelativeMouseState(&position.x, &position.y);
    camera->Yaw += (sensitivity * (float)position.x);
    camera->Pitch += (sensitivity * (float)position.y);
  }

  // update the camera and set the various shader uniforms accordingly...
  {
    camera->Update();
  }

  // update the object(s)...
  {
    const float rotationsPerSecond = 90.0f;
    rotation += rotationsPerSecond * elapsedSeconds;
    if (rotation > 360.0f) { rotation -= 360.0f; }
    instances.front()->transform = glm::rotate(glm::mat4(1), rotation, glm::vec3(0,1,0));
  }
}

//-----------------------------------------------------
static void Render()
{
  RenderState renderState;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  uniformBuffer->Enable();
  globalUniforms.CameraPos = glm::vec4(camera->Position, 1);
  globalUniforms.ViewMatrix = camera->ViewMatrix;
  globalUniforms.ProjectionMatrix = camera->PerspectiveMatrix;
  globalUniforms.InverseViewMatrix = glm::inverse(globalUniforms.ViewMatrix);

  BOOST_FOREACH(boost::shared_ptr<ModelInstance> instance, instances)
  {
    renderState.vao = instance->asset->vao;
    renderState.shader = instance->asset->shader;

    renderState.textureUnits[0].active = true;
    renderState.textureUnits[0].sampler = instance->asset->sampler;
    renderState.textureUnits[0].texture = instance->asset->texture;

    globalUniforms.WorldMatrix = instance->transform;

    globalUniforms.WorldViewMatrix = globalUniforms.ViewMatrix * globalUniforms.WorldMatrix;
    globalUniforms.ViewProjectionMatrix = globalUniforms.ProjectionMatrix * globalUniforms.ViewMatrix;
    globalUniforms.WorldViewProjectionMatrix = globalUniforms.ViewProjectionMatrix * globalUniforms.WorldMatrix;

    uniformBuffer->SetData(&globalUniforms, sizeof(globalUniforms), 0);

    ApplyTextureUnits(renderState.textureUnits, device.renderState.textureUnits);
    ApplyShader(renderState.shader, device.renderState);
    ApplyVAO(renderState.vao, device.renderState);

    glDrawArrays(instance->asset->drawType, instance->asset->drawFirstVertex, instance->asset->drawVertexCount);
  }

  SDL_GL_SwapWindow(mainWindow);
}

//-----------------------------------------------------
static void CreateInstances()
{
  boost::shared_ptr<ModelInstance> instance;

  /* dot over the 'i'*/
  instance = boost::make_shared<ModelInstance>();
  instance->asset = woodenCrate;
  instance->transform = glm::mat4();
  instances.push_back(instance);

  /* i */
  instance = boost::make_shared<ModelInstance>();
  instance->asset = woodenCrate;
  instance->transform = glm::translate(0.0f, -4.0f, 0.0f) * glm::scale(1.0f, 2.0f, 1.0f);
  instances.push_back(instance);

  /* left arm of H */
  instance = boost::make_shared<ModelInstance>();
  instance->asset = woodenCrate;
  instance->transform = glm::translate(-8.0f, 0.0f, 0.0f) * glm::scale(1.0f, 6.0f, 1.0f);
  instances.push_back(instance);

  /* right arm of H */
  instance = boost::make_shared<ModelInstance>();
  instance->asset = woodenCrate;
  instance->transform = glm::translate(-4.0f, 0.0f, 0.0f) * glm::scale(1.0f, 6.0f, 1.0f);
  instances.push_back(instance);

  /* middle bar of H */
  instance = boost::make_shared<ModelInstance>();
  instance->asset = woodenCrate;
  instance->transform = glm::translate(-6.0f, 0.0f, 0.0f) * glm::scale(2.0f, 1.0f, 0.8f);
  instances.push_back(instance);
}

//-----------------------------------------------------
static void ForceRenderState(const RenderState& state)
{
  glColorMask(state.colourMask.r, state.colourMask.g, state.colourMask.b, state.colourMask.a);
  glDepthMask(state.depthMask);
  
  if (state.depthTestEnabled)
  {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(state.depthFunc);
  }
  else
  {
    glDisable(GL_DEPTH_TEST);
  }

  if (state.enableCulling)
  {
    glEnable(GL_CULL_FACE);
    glCullFace(state.cullFace);
    glFrontFace(state.frontFace);
  }
  else
  {
    glDisable(GL_CULL_FACE);
  }

  for (size_t i = 0; i < RenderState::MaxTextureUnits; ++i)
  {
    glActiveTexture(GL_TEXTURE0 + i);
    Sampler2D::Unbind(i);
  }
}

//-----------------------------------------------------
static void ForceClearState(const ClearState& state)
{
  glClearColor(state.colour.r, state.colour.g, state.colour.b, state.colour.a);
  glClearDepth(state.depth);
}

//-----------------------------------------------------
static void ApplyVAO(GLuint updated, RenderState& state)
{
  if (updated != state.vao)
  {
    state.vao = updated;
    glBindVertexArray(state.vao);
  }
}

//-----------------------------------------------------
static void ApplyShader(boost::shared_ptr<Shader> shader, RenderState& state)
{
  if (state.shader != shader)
  {
    state.shader = shader;
    state.shader->Use();
  }
  state.shader->ApplyUniforms();
}

//-----------------------------------------------------
static void ApplyTextureUnits(TextureUnit updated[], TextureUnit current[])
{
  for (size_t i = 0; i < RenderState::MaxTextureUnits; ++i)
  {
    if (current[i] != updated[i])
    {
      glActiveTexture(GL_TEXTURE0 + i);
      if (updated[i].active)
      {
        if (updated[i].texture != current[i].texture) { updated[i].texture->Activate(); }
        if (updated[i].sampler != current[i].sampler) { updated[i].sampler->Bind(i); }
      }
      else
      {
        Sampler2D::Unbind(i);
      }

      current[i] = updated[i];
    }
  }
}
