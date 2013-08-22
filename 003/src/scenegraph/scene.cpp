#include <SDL.h>
#include <scenegraph/scene.h>

Scene::Scene(boost::shared_ptr<Device> device)
	: device(device)
{
}

Scene::~Scene()
{
}

void Scene::Load()
{
	if (root) { root->Load(this); }
}

void Scene::Update()
{
	static unsigned int prevTime = 0;

	if (root)
	{
		const unsigned int now = SDL_GetTicks();

		if (0 == prevTime) { prevTime = now; }

		const float elapsedMS = (float)(now - prevTime) / 1000.0f;
		prevTime = now;

		root->Update(this, elapsedMS);
	}
}

void Scene::Render()
{
	if (root)
	{
		if (root->PreRender(this))
		{
			root->Render(this);
			root->RenderChildren(this);
			root->PostRender(this);
		}
	}
}
