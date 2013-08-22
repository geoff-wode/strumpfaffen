#include <scenegraph/scenenode.h>
#include <glm/ext.hpp>

//--------------------------------------------------------------------
SceneNode::SceneNode()
{
}

//--------------------------------------------------------------------
SceneNode::~SceneNode()
{
	while (!children.empty())
	{
		children.pop_front();
	}
}

//--------------------------------------------------------------------

void SceneNode::Load(Scene* const scene)
{
	for (SceneNodeList::iterator i = children.begin(); i != children.end(); ++i)
	{
		(*i)->Load(scene);
	}
}

//--------------------------------------------------------------------
void SceneNode::RenderChildren(Scene* const scene)
{
	for (SceneNodeList::iterator i = children.begin(); i != children.end(); ++i)
	{
		if ((*i)->PreRender(scene))
		{
			(*i)->Render(scene);
			(*i)->RenderChildren(scene);
			(*i)->PostRender(scene);
		}
	}
}

//--------------------------------------------------------------------
void SceneNode::Update(Scene* const scene, float elapsedMS)
{
	for (SceneNodeList::iterator i = children.begin(); i != children.end(); ++i)
	{
		(*i)->Update(scene, elapsedMS);
	}
}

//--------------------------------------------------------------------
bool SceneNode::PreRender(Scene* const scene)
{
	return true;
}

//--------------------------------------------------------------------
void SceneNode::Render(Scene* const scene)
{
}

//--------------------------------------------------------------------
void SceneNode::PostRender(Scene* const scene)
{
}

//--------------------------------------------------------------------
void SceneNode::SetTransform(const glm::mat4& toWorld)
{
	SetTransform(toWorld, glm::inverse(toWorld));
}

//--------------------------------------------------------------------
void SceneNode::SetTransform(const glm::mat4& toWorld, const glm::mat4& fromWorld)
{
}

//--------------------------------------------------------------------
