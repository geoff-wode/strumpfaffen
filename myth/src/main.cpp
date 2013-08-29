#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <glm/glm.hpp>
#include <gl_loader/gl_loader.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <debug.h>
#include <shader.h>
#include <buffers.h>

//-----------------------------------------------------

FILE* debug::logFile;
static SDL_Window* mainWindow;
static SDL_GLContext glContext;

static GLuint vao;
static boost::shared_ptr<VertexBuffer>  vertexBuffer;
boost::shared_ptr<Shader> shader;

//-----------------------------------------------------

static void Init(const std::string& title, int width, int height, bool fullScreen);
static void CreateTriangle();
static void Render();

//-----------------------------------------------------

int main(int argc, char* argv[])
{
  Init("Myth", 1280, 720, false);

  shader = boost::make_shared<Shader>("shaders/simplest");
  CreateTriangle();

  bool quit = false;

  while (!quit)
  {
    {
      SDL_Event event;
      while (SDL_PollEvent(&event))
      {
        switch (event.type)
        {
        case SDL_QUIT: quit = true; break;
        default: break;
        }
      }
    }
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
    mainWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
  }
  else
  {
    mainWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
  }

	glContext = SDL_GL_CreateContext(mainWindow);

	ogl_LoadFunctions();
  LOG("GL v%d.%d\n", ogl_GetMajorVersion(), ogl_GetMinorVersion());
  LOG("GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
  LOG("Vendor: %s\n", glGetString(GL_VENDOR));
  LOG("Renderer: %s\n", glGetString(GL_RENDERER));

  glClearColor(0, 0, 0, 1);
}

//-----------------------------------------------------
static void CreateTriangle()
{
  static const glm::vec3 vertices[] =
  {
    glm::vec3(-0.75f, -0.75f, 0.0f),
    glm::vec3( 0.75f, -0.75f, 0.0f),
    glm::vec3( 0.0f,   0.75f, 0.0f),
  };

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  vertexBuffer = boost::make_shared<VertexBuffer>(sizeof(vertices), GL_STATIC_DRAW);
  vertexBuffer->SetData(vertices, sizeof(vertices), 0);
  vertexBuffer->Enable();

  const GLint attribIndex = shader->GetAttributeIndex("Position");
  glEnableVertexAttribArray(attribIndex);
  glVertexAttribPointer(attribIndex, 3, GL_FLOAT, GL_FALSE, 0, (const void*)0);

  VertexBuffer::Disable();
  glBindVertexArray(0);
}

//-----------------------------------------------------
static void Render()
{
  glClear(GL_COLOR_BUFFER_BIT);

  shader->Use();
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glBindVertexArray(0);
  glUseProgram(0);

  SDL_GL_SwapWindow(mainWindow);
}
