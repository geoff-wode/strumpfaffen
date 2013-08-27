#include <scenegraph/scene.h>
#include <scenegraph/scenenode.h>
#include <boost/foreach.hpp>

//-----------------------------------------------------------

SceneNode::SceneNode(SceneNodePtr parent)
	: parent(parent)
{
}

SceneNode::~SceneNode()
{
	while (!children.empty())
	{
		children.pop_front();
	}
}

//-----------------------------------------------------------

void SceneNode::LoadContent(Scene* const scene)
{
	BOOST_FOREACH(auto node, children)
	{
		node->LoadContent(scene);
	}
}

//-----------------------------------------------------------

void SceneNode::Update(Scene* const scene, unsigned int elapsedMS)
{
	BOOST_FOREACH(auto node, children)
	{
		node->Update(scene, elapsedMS);
	}
}

//-----------------------------------------------------------

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

//-----------------------------------------------------------

void SceneNode::SetTransform(const glm::mat4& toWorld)
{
	SetTransform(toWorld, glm::inverse(toWorld));
}

void SceneNode::SetTransform(const glm::mat4& toWorld, const glm::mat4& fromWorld)
{
}

//-----------------------------------------------------------
void SceneNode::RenderChildren(Scene* const scene)
{
	BOOST_FOREACH(auto node, children)
	{
		if (node->PreRender(scene))
		{
			node->Render(scene);
			node->RenderChildren(scene);
			node->PostRender(scene);
		}
	}
}
