#include <scenegraph/scene.h>

//-----------------------------------------------------------

Scene::Scene(Device& device)
	: device(device)
{
}

Scene::~Scene()
{
}

//-----------------------------------------------------------

void Scene::LoadContent()
{
	if (root) { root->LoadContent(this); }
}

void Scene::Update(unsigned int elapsedMS)
{
	if (root) { root->Update(this, elapsedMS); }
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
