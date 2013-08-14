#include <framework/scenegraph/scenenode.h>

using namespace Framework;

//---------------------------------------------------------------------------

SceneNode::SceneNode()
{
}

SceneNode::~SceneNode()
{
	while (!childNodes.empty())
	{
		childNodes.pop_front();
	}
}

void SceneNode::LoadResources(Scene* const scene)
{
	for (SceneNodeList::const_iterator i = childNodes.begin(); i != childNodes.end(); ++i)
	{
		(*i)->LoadResources(scene);
	}
}

void SceneNode::Update(Scene* const scene, unsigned int elapsedMS)
{
	for (SceneNodeList::const_iterator i = childNodes.begin(); i != childNodes.end(); ++i)
	{
		(*i)->Update(scene, elapsedMS);
	}
}

void SceneNode::RenderChildren(Scene* const scene)
{
	for (SceneNodeList::const_iterator i = childNodes.begin(); i != childNodes.end(); ++i)
	{
		if ((*i)->PreRender(scene))
		{
			(*i)->Render(scene);
			(*i)->RenderChildren(scene);
			(*i)->PostRender(scene);
		}
	}
}

bool SceneNode::PreRender(Scene* const scene)
{
	return true;
}

void SceneNode::Render(Scene* const scene)
{
}

void SceneNode::PostRender(Scene* const scene)
{
}

void SceneNode::SetTransform(const glm::mat4& toWorld)
{
}

void SceneNode::SetTransform(const glm::mat4& toWorld, const glm::mat4& fromWorld)
{
}