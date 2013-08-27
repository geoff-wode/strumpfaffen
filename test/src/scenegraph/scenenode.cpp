#include <scenegraph/scene.h>
#include <scenegraph/scenenode.h>
#include <boost/foreach.hpp>

//-----------------------------------------------------------

SceneNode::SceneNode()
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

void SceneNode::LoadNodeContent(Scene* const scene)
{
	// Make a copy of the child list in case this node wants to modify it in some way...
	const SceneNodeList listCopy(children);

	LoadContent(scene);

	BOOST_FOREACH(auto node, children)
	{
		node->LoadNodeContent(scene);
	}
}

//-----------------------------------------------------------

void SceneNode::UpdateNode(Scene* const scene, unsigned int elapsedMS)
{
	// Make a copy of the child list in case this node wants to modify it in some way...
	const SceneNodeList listCopy(children);

	Update(scene, elapsedMS);

	BOOST_FOREACH(auto node, listCopy)
	{
		node->UpdateNode(scene, elapsedMS);
	}
}

//-----------------------------------------------------------
void SceneNode::RenderNode(Scene* const scene)
{
	if (PreRender(scene))
	{
		Render(scene);
		BOOST_FOREACH(auto node, children)
		{
			node->RenderNode(scene);
		}
		PostRender(scene);
	}
}
