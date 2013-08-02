#include <scenegraph/scenenode.h>

using namespace SceneGraph;

//-----------------------------------------------------------------------

SceneNodeClass::SceneNodeClass()
{
}

SceneNodeClass::~SceneNodeClass()
{
	while (!Children.empty())
	{
		Children.pop_back();
	}
}

void SceneNodeClass::RenderChildren(Scene* const scene)
{
	for (NodeList::iterator i = Children.begin(); i != Children.end(); ++i)
	{
		SceneNode node = *i;
		if (node->PreRender(scene))
		{
			node->Render(scene);
			node->RenderChildren(scene);
			node->PostRender(scene);
		}
	}
}

void SceneNodeClass::LoadResources(Scene* const scene)
{
	for (NodeList::iterator i = Children.begin(); i != Children.end(); ++i)
	{
		SceneNode node = *i;
		node->LoadResources(scene);
	}
}

void SceneNodeClass::Update(unsigned int elapsedMS, Scene* const scene)
{
	for (NodeList::iterator i = Children.begin(); i != Children.end(); ++i)
	{
		SceneNode node = *i;
		node->Update(elapsedMS, scene);
	}
}

bool SceneNodeClass::PreRender(Scene* const scene)
{
	return true;
}

void SceneNodeClass::Render(Scene* const scene)
{
}

void SceneNodeClass::PostRender(Scene* const scene)
{
}

void SceneNodeClass::SetTransform(const glm::mat4& toWorld)
{
}

void SceneNodeClass::SetTransform(const glm::mat4& toWorld, const glm::mat4& fromWorld)
{
}
