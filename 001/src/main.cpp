#include <SDL.h>
#include <stdlib.h>
#include <debug.h>
#include <gl/gl_loader.h>
#include <game.h>
#include <display.h>

//----------------------------------------------------------

static void InitSystem(int width, int height);

//----------------------------------------------------------

DisplayData display;

//----------------------------------------------------------

int main(int argc, char* argv[])
{
	Game game;

	InitSystem(display.width, display.height);

	if (game.Init() && game.Load())
	{
		const float frameRate = 1000.0f / 60.0f;
		float previousTime = 0.0f;
		while (game.IsRunning())
		{
			float now = (float)SDL_GetTicks();
			float deltaMS = (now - previousTime) / 1000.0f;
			if (deltaMS > frameRate) { deltaMS = frameRate; }
			previousTime = now;

			game.Update(deltaMS);
			display.Update();

			if (game.IsRunning())
			{
				game.Render(deltaMS);
				SDL_GL_SwapBuffers();	
			}
		}
	}

	return 0;
}

//----------------------------------------------------------

static void InitSDL()
{
  static const struct SDLAttribute
  {
    const char* name;
    SDL_GLattr  attr;
    int         value;
  } sdlAttributes[] =
  {
    { "SDL_GL_DOUBLEBUFFER",  SDL_GL_DOUBLEBUFFER,            1 },
    { "SDL_GL_RED_SIZE",      SDL_GL_RED_SIZE,                8 },
    { "SDL_GL_GREEN_SIZE",    SDL_GL_GREEN_SIZE,              8 },
    { "SDL_GL_BLUE_SIZE",     SDL_GL_BLUE_SIZE,               8 },
    { "SDL_GL_ALPHA_SIZE",    SDL_GL_ALPHA_SIZE,              8 },
    { "SDL_GL_BUFFER_SIZE",   SDL_GL_BUFFER_SIZE,             24 },
    { "SDL_GL_MULTISAMPLEBUFFERS", SDL_GL_MULTISAMPLEBUFFERS, 1 },
    { "SDL_GL_MULTISAMPLESAMPLES", SDL_GL_MULTISAMPLESAMPLES, 4 }
  };
  const int NumAttributes = sizeof(sdlAttributes)/sizeof(sdlAttributes[0]);
  for (int i = 0; i < NumAttributes; ++i)
  {
    SDL_GL_SetAttribute(sdlAttributes[i].attr, sdlAttributes[i].value);
  }
}

static void InitGL()
{
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
}

static void InitSystem(int width, int height)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	atexit(SDL_Quit);
	InitSDL();
  if (NULL == SDL_SetVideoMode(width, height, 32, SDL_OPENGL | SDL_HWSURFACE))
  {
    exit(EXIT_FAILURE);
  }
	InitSDL(); // workaround for occassional bug in some SDL implementations
	ogl_LoadFunctions();
  LOG("GL v%d.%d\n", ogl_GetMajorVersion(), ogl_GetMinorVersion());
  LOG("GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	InitGL();

	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}
