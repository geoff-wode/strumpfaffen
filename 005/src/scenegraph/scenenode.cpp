#include <scenegraph/scenenode.h>
#include <boost/foreach.hpp>

//---------------------------------------------------------------------

SceneNode::~SceneNode()
{
	while (!children.empty())
	{
		children.pop_front();
	}
}

//---------------------------------------------------------------------
void SceneNode::LoadContent(Scene* const scene)
{
	// Load this node's content...
	LoadNodeContent(scene);

	// ...and child node's content...
	BOOST_FOREACH(SceneNodePtr node, children)
	{
		node->LoadContent(scene);
	}
}

//---------------------------------------------------------------------
void SceneNode::UpdateState(Scene* const scene, unsigned int elapsedMS)
{
	// Update this node...
	UpdateNodeState(scene, elapsedMS);

	// ...then child nodes...
	BOOST_FOREACH(SceneNodePtr node, children)
	{
		node->UpdateState(scene, elapsedMS);
	}
}

//---------------------------------------------------------------------
void SceneNode::Render(Scene* const scene)
{
	// Render this node and child nodes (depending on whether the pre-render
	// step is successful or not)...
	if (PreRenderNode(scene))
	{
		RenderNode(scene);

		BOOST_FOREACH(SceneNodePtr node, children)
		{
				node->Render(scene);
		}

		// Call the post-render step on this node...
		PostRenderNode(scene);
	}
}

//---------------------------------------------------------------------
