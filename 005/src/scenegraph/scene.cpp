#include <scenegraph/scene.h>

//---------------------------------------------------------------------

Scene::Scene()
{
}

Scene::~Scene()
{
}

//---------------------------------------------------------------------

void Scene::LoadContent()
{
	if (rootNode)
	{
		rootNode->LoadContent(this);
	}
}

//---------------------------------------------------------------------
void Scene::Update(unsigned int elapsedMS)
{
	if (rootNode)
	{
		rootNode->UpdateState(this, elapsedMS);
	}
}

//---------------------------------------------------------------------
void Scene::Render()
{
	if (rootNode)
	{
		rootNode->Render(this);
	}
}
