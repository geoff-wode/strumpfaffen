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
#include <input/keyboard.h>

//--------------------------------------------------------------------------------------------

static void Init();
static boost::shared_ptr<Scene> BuildScene(Device& device);
static void PollEvents(Scene* const scene);

//--------------------------------------------------------------------------------------------

FILE*	debugLogFile;
static SDL_Window*		mainWindow;
static SDL_GLContext	glContext;
static bool						quit = false;

//--------------------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	Init();
	Device device;

	device.ProjectionMatrix = glm::perspective(45.0f, 800.0f/600.0f, 0.1f, 100.0f);

	boost::shared_ptr<Scene> scene = BuildScene(device);

	boost::shared_ptr<CameraNode> camera(new CameraNode());
	camera->SetTransform(glm::mat4(glm::translate(0,1,4)));
	scene->root->children.push_back(camera);
	
	scene->movementController = boost::make_shared<MovementController>(camera);

	scene->LoadContent();

	unsigned int prevTime = 0;

	do
	{
		PollEvents(scene.get());

		// Compute how much time has elapsed since the last frame...
		const unsigned int now(SDL_GetTicks());
		if (0 == prevTime) { prevTime = now; }
		const unsigned int elapsedMS(now - prevTime);
		prevTime = now;

		// Update internal state...
		scene->Update(elapsedMS);

		// TODO: The scene should be deciding when/how to clear any buffers...
		glClear(GL_COLOR_BUFFER_BIT);
		
		// Light up some pixels - hopefully not in black on a black background...
		scene->Render();

		// Flipping (is) marvellous...
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
}

//--------------------------------------------------------------------------------------------
static void PollEvents(Scene* const scene)
{
	SDL_Event event;
	while (!quit && SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT: quit = true; break;
		case SDL_KEYDOWN:
			{
				if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				{
					quit = true;
				}
				else
				{
					scene->movementController->OnKeyDown(event.key.keysym.scancode);
				}
			}
			break;
		case SDL_KEYUP:
			{
				scene->movementController->OnKeyUp(event.key.keysym.scancode);
			}
			break;
		case SDL_MOUSEMOTION:
			{
				// A work-around due to sometimes getting an error-filled event after explicitly
				// setting the mouse position...
				static bool firstTime = true;
				if (!firstTime)
				{
					scene->movementController->OnMouseMove(glm::ivec2(event.motion.xrel, event.motion.yrel));
				}
				else
				{
					firstTime = false;
				}
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			{
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT: scene->movementController->OnMouseLeftButtonDown(glm::ivec2(event.button.x, event.button.y)); break;
				case SDL_BUTTON_RIGHT: scene->movementController->OnMouseRightButtonDown(glm::ivec2(event.button.x, event.button.y)); break;
				default: break;
				}
			}
			break;
		case SDL_MOUSEBUTTONUP:
			{
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT: scene->movementController->OnMouseLeftButtonUp(glm::ivec2(event.button.x, event.button.y)); break;
				case SDL_BUTTON_RIGHT: scene->movementController->OnMouseRightButtonUp(glm::ivec2(event.button.x, event.button.y)); break;
				default: break;
				}
			}
			break;
		default: break;
		}
	}
}

//--------------------------------------------------------------------------------------------

static boost::shared_ptr<Scene> BuildScene(Device& device)
{
	boost::shared_ptr<Scene> scene(new Scene(device));

	scene->root = boost::make_shared<TransformNode>();

	boost::shared_ptr<ShaderNode> shader(new ShaderNode("shaders/simple"));
	boost::shared_ptr<TileNode> tile(new TileNode(5));

	scene->root->children.push_back(shader);

	shader->children.push_back(tile);

	return scene;
}
