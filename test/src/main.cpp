#include <memory>
#include <vector>
#include <string>
#include <map>
#include <stdio.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <gl_loader/gl_loader.h>
#include <SDL.h>

#include <device.h>
#include <shader.h>
#include <scenegraph/scene.h>
#include <scenegraph/cameranode.h>
#include <scenegraph/shadernode.h>
#include <scenegraph/terrain/tilenode.h>
#include <input/keyboard.h>
#include <camera.h>

//--------------------------------------------------------------------------------------------

static void Init();
static boost::shared_ptr<Scene> BuildScene(Device& device);
static void PollEvents();

//--------------------------------------------------------------------------------------------

static const glm::ivec2 screenSize(800, 600);

//--------------------------------------------------------------------------------------------

FILE*	debugLogFile;
static SDL_Window*		mainWindow;
static SDL_GLContext	glContext;

//--------------------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	Init();
	Device device;

	//device.ProjectionMatrix = glm::perspective(45.0f, 800.0f/600.0f, 0.1f, 100.0f);

	boost::shared_ptr<Scene> scene = BuildScene(device);

	scene->LoadContent();

	do
	{
    PollEvents();

		scene->Update();

		// TODO: The scene should be deciding when/how to clear any buffers...
		glClear(GL_COLOR_BUFFER_BIT);
		
		// Light up some pixels - hopefully not in black on a black background...
		scene->Render();

		// Flipping (is) marvellous...
		SDL_GL_SwapWindow(mainWindow);
	} while (!scene->HasQuit());

	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(mainWindow);
	return 0;
}

//--------------------------------------------------------------------------------------------
static void Init()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	atexit(SDL_Quit);

	debugLogFile = fopen("stderr.txt", "wb");

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

	mainWindow = SDL_CreateWindow("Strumpfaffen", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	glContext = SDL_GL_CreateContext(mainWindow);
	ogl_LoadFunctions();
}

//--------------------------------------------------------------------------------------------

static boost::shared_ptr<Scene> BuildScene(Device& device)
{
	boost::shared_ptr<Scene> scene(new Scene(device));

	scene->root = boost::make_shared<TransformNode>();

	boost::shared_ptr<ShaderNode> shader(new ShaderNode("shaders/simple"));
	scene->root->children.push_back(shader);

	boost::shared_ptr<TileNode> tile(new TileNode(5));
	shader->children.push_back(tile);

	return scene;
}

//--------------------------------------------------------------------------------------------
static void PollEvents()
{
	SDL_Event event;
	while (!quit && SDL_PollEvent(&event))
	{
		if (SDL_QUIT == event.type)
		{
  		quit = true;
		}
	}
}
