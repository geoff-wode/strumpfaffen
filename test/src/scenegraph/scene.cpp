#include <SDL.h>
#include <scenegraph/scene.h>

//-----------------------------------------------------------

Scene::Scene(Device& device)
	: device(device),
		quit(false),
		prevTime(0)
{
}

Scene::~Scene()
{
}

//-----------------------------------------------------------

void Scene::LoadContent()
{
	if (root) { root->LoadNodeContent(this); }
}

void Scene::Update()
{
	// Compute how much time has elapsed since the last frame...
	const float now((float)SDL_GetTicks());
	if (0 == prevTime) { prevTime = now; }
	const float elapsedTime(now - prevTime);
	prevTime = now;

  // Handle keyboard control of the camera...
  {
    const Uint8* keyState = SDL_GetKeyboardState(NULL);
    if (keyState[SDL_SCANCODE_ESCAPE]) { Quit(); return; }
    if (keyState[SDL_SCANCODE_W]) { camera->Translate(elapsedTime * camera->Forward); }
    if (keyState[SDL_SCANCODE_S]) { camera->Translate(elapsedTime * -camera->Forward); }
    if (keyState[SDL_SCANCODE_A]) { camera->Translate(elapsedTime * -camera->Right); }
    if (keyState[SDL_SCANCODE_D]) { camera->Translate(elapsedTime * camera->Right); }
    // Use the world "up" NOT the camera's local "up" to move the camera...
    if (keyState[SDL_SCANCODE_Q]) { camera->Translate(elapsedTime * glm::vec3(0,1,0)); }
    if (keyState[SDL_SCANCODE_E]) { camera->Translate(elapsedTime * glm::vec3(0,-1,0)); }
  }

	if (root)
	{
		root->UpdateNode(this, elapsedTime);
	}
}

//--------------------------------------------------------------------------------------------
void Scene::Render()
{
	if (root)
	{
		root->RenderNode(this);
	}
}

//--------------------------------------------------------------------------------------------
