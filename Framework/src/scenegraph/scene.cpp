#include <scenegraph/scene.h>
#include <scenegraph/scenenode.h>

using namespace SceneGraph;

//-----------------------------------------------------------------------

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::LoadResources()
{
	if (Root)
	{
		Root->LoadResources(this);
	}
}

void Scene::Update(unsigned int elapsedMS)
{
	if (Root)
	{
		Root->Update(elapsedMS, this);
	}
}

void Scene::Render()
{
	if (Root && Root->PreRender(this))
	{
		Root->Render(this);
		Root->RenderChildren(this);
		Root->PostRender(this);
	}
}
