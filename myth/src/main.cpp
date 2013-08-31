#include <SDL.h>
#include <SOIL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <gl_loader/gl_loader.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <debug.h>
#include <shader.h>
#include <buffers.h>
#include <texture.h>
#include <sampler.h>
#include <camera.h>

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

//-----------------------------------------------------

FILE* debug::logFile;
static SDL_Window* mainWindow;
static SDL_GLContext glContext;

static boost::shared_ptr<UniformBuffer> uniformBuffer;
static GlobalUniforms                   globalUniforms;

static GLuint vao;
static boost::shared_ptr<VertexBuffer>  vertexBuffer;

static boost::shared_ptr<Camera> camera;
static boost::shared_ptr<Shader> shader;
static boost::shared_ptr<Texture2D> texture;
static boost::shared_ptr<Sampler2D> sampler;

static float rotation = 0.0f;
static glm::ivec2 oldMousePos;

//-----------------------------------------------------

static void Init(const std::string& title, int width, int height, bool fullScreen);
static void CreateObject();
static void Update(unsigned int elapsedMS);
static void Render();

//-----------------------------------------------------

int main(int argc, char* argv[])
{
  Init("Myth", 1280, 720, false);
  
  uniformBuffer = boost::make_shared<UniformBuffer>(sizeof(GlobalUniforms), 0);

  const float aspectRatio((float)1280/(float)720);
  camera = boost::make_shared<Camera>(45.0f, aspectRatio, 0.1f, 100.0f);
  camera->Position = glm::vec3(0,2,4);

  shader = boost::make_shared<Shader>("shaders/textured");
  shader->SetUniform("sampler", 0);

  texture = boost::make_shared<Texture2D>("images/wooden-crate.jpg");
  texture->Load();

  sampler = boost::make_shared<Sampler2D>(0);
  sampler->SetTexture(texture);

  CreateObject();

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

  glClearColor(0, 0, 0, 1);
  glClearDepth(1.0);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  SDL_GetMouseState(&oldMousePos.x, &oldMousePos.y);
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

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  vertexBuffer = boost::make_shared<VertexBuffer>(sizeof(vertices), GL_STATIC_DRAW);
  vertexBuffer->Enable();
  vertexBuffer->SetData(vertices, sizeof(vertices), 0);

  glEnableVertexAttribArray(shader->GetAttributeIndex("Position"));
  glVertexAttribPointer(shader->GetAttributeIndex("Position"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));

  glEnableVertexAttribArray(shader->GetAttributeIndex("TextureCoord"));
  glVertexAttribPointer(shader->GetAttributeIndex("TextureCoord"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, textureCoord));

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
    globalUniforms.CameraPos = glm::vec4(camera->Position, 1);
    globalUniforms.ViewMatrix = camera->ViewMatrix;
    globalUniforms.ProjectionMatrix = camera->PerspectiveMatrix;
  }

  // update the object(s)...
  {
    const float rotationsPerSecond = 90.0f;
    rotation += rotationsPerSecond * elapsedSeconds;
    if (rotation > 360.0f) { rotation -= 360.0f; }
    globalUniforms.WorldMatrix = glm::rotate(glm::mat4(1), rotation, glm::vec3(0,1,0));
  }

  // compute "automatic" shader uniforms...
  {
    globalUniforms.InverseViewMatrix = glm::inverse(globalUniforms.ViewMatrix);
    globalUniforms.WorldViewMatrix = globalUniforms.ViewMatrix * globalUniforms.WorldMatrix;
    globalUniforms.ViewProjectionMatrix = globalUniforms.ProjectionMatrix * globalUniforms.ViewMatrix;
    globalUniforms.WorldViewProjectionMatrix = globalUniforms.ViewProjectionMatrix * globalUniforms.WorldMatrix;
  }
}

//-----------------------------------------------------
static void Render()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  uniformBuffer->Enable();
  uniformBuffer->SetData(&globalUniforms, sizeof(globalUniforms), 0);

  shader->Use();
  sampler->Activate();

  glBindVertexArray(vao);

  glDrawArrays(GL_TRIANGLES, 0, 6 * 3 * 2);
  
  Texture2D::Deactivate();
  glBindVertexArray(0);
  glUseProgram(0);

  SDL_GL_SwapWindow(mainWindow);
}

