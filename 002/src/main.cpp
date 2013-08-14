#include <SDL.h>
#include <glm/glm.hpp>
#include <boost/make_shared.hpp>

#include <framework/device.h>
#include <framework/scenegraph/scene.h>
#include <framework/shaders/shader.h>

//-----------------------------------------------------------------

static void ManageEvents();
static void KeyDownHandler(SDLKey key);

//-----------------------------------------------------------------

static bool quit = false;
static const Framework::ClearState clearState;


//-----------------------------------------------------------------

int main(int argc, char* argv[])
{
	Framework::Device device(glm::ivec2(800, 600));

	if (!device.Initialise())
	{
		exit(EXIT_FAILURE);
	}

	Framework::SceneNodePtr rootNode(new Framework::TransformNode());
	Framework::SceneNodePtr camera(new Framework::CameraNode());

	rootNode->childNodes.push_back(camera);

	Framework::Input::MovementControllerPtr movementController(new Framework::Input::MovementController());
	movementController->SetObject(camera);
	movementController->OnKeyDown = KeyDownHandler;

	Framework::Scene scene(&device);
	scene.root = rootNode;
	scene.movementController = movementController;

	scene.root->childNodes.push_back(boost::make_shared<Framework::MeshNode>());

	scene.LoadResources();

	while (!quit)
	{
		ManageEvents();
		if (!quit)
		{
			scene.device->Clear(clearState);
			scene.Update();
			scene.Render();
			SDL_GL_SwapBuffers();
		}
	}

	return 0;
}

//-----------------------------------------------------------------

static void ManageEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT: quit = true; break;
		}
	}
}

static void KeyDownHandler(SDLKey key)
{
	if (SDLK_ESCAPE == key) { quit = true; }
}
