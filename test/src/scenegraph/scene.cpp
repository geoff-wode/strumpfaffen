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
	if (root) { root->LoadNodeContent(this); }
}

void Scene::Update(unsigned int elapsedMS)
{
	if (movementController)
	{
		movementController->Update(elapsedMS);
	}
	if (root)
	{
		root->UpdateNode(this, elapsedMS);
	}
}

void Scene::Render()
{
	if (root)
	{
		root->RenderNode(this);
	}
}
