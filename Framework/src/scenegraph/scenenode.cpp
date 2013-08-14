#include <scenegraph/scenenode.h>

using namespace SceneGraph;

//-----------------------------------------------------------------------

SceneNode::SceneNode()
{
}

SceneNode::~SceneNode()
{
	while (!Children.empty())
	{
		Children.pop_back();
	}
}

void SceneNode::RenderChildren(Scene* const scene)
{
	for (NodeList::iterator i = Children.begin(); i != Children.end(); ++i)
	{
		boost::shared_ptr<class SceneNode> node = *i;
		if (node->PreRender(scene))
		{
			node->Render(scene);
			node->RenderChildren(scene);
			node->PostRender(scene);
		}
	}
}

void SceneNode::LoadResources(Scene* const scene)
{
	for (NodeList::iterator i = Children.begin(); i != Children.end(); ++i)
	{
		boost::shared_ptr<class SceneNode> node = *i;
		node->LoadResources(scene);
	}
}

void SceneNode::Update(Scene* const scene, unsigned int elapsedMS)
{
	for (NodeList::iterator i = Children.begin(); i != Children.end(); ++i)
	{
		boost::shared_ptr<class SceneNode> node = *i;
		node->Update(scene, elapsedMS);
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
