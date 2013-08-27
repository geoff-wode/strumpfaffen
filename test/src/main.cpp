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
#include <movementcontroller.h>

//--------------------------------------------------------------------------------------------

static void Init();

//--------------------------------------------------------------------------------------------

FILE* debugLogFile;
static SDL_Window* mainWindow;
static SDL_GLContext glContext;

//--------------------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	Init();
	Device device;

	//device.WorldMatrix = glm::translate(glm::vec3(0,0,0));
	//device.ViewMatrix = glm::lookAt(glm::vec3(0,2,4), glm::vec3(0), glm::vec3(0,1,0));
	device.ProjectionMatrix = glm::perspective(45.0f, 800.0f/600.0f, 0.1f, 100.0f);

	Scene scene(device);
	scene.root = boost::make_shared<TransformNode>();

	boost::shared_ptr<CameraNode> camera(new CameraNode());
	boost::shared_ptr<ShaderNode> shader(new ShaderNode("shaders/simple"));
	boost::shared_ptr<TileNode> tile(new TileNode(5));

	camera->SetTransform(glm::mat4(glm::translate(0,2,4)));

	scene.root->children.push_back(camera);
	scene.root->children.push_back(shader);

	shader->children.push_back(tile);

	scene.movementController = boost::make_shared<MovementController>(camera);

	scene.LoadContent();

	bool quit = false;
	do
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT: quit = true; break;
			case SDL_KEYDOWN: if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) { quit = true; }
			default: break;
			}
		}

		scene.Update(0);

		glClear(GL_COLOR_BUFFER_BIT);
		
		scene.Render();

		SDL_GL_SwapWindow(mainWindow);
	} while (!quit);

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

	glClearColor(0, 0, 0, 1);
	glColorMask(true, true, true, true);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);

	glPolygonMode(GL_FRONT, GL_LINE);
}

//--------------------------------------------------------------------------------------------
