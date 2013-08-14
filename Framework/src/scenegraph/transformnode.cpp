#include <scenegraph/transformnode.h>
#include <scenegraph/scene.h>

using namespace SceneGraph;

//------------------------------------------------------------------------------------

TransformNode::TransformNode()
	: ToWorld(1), FromWorld(1)
{
}

TransformNode::~TransformNode()
{
}

bool TransformNode::PreRender(Scene* const scene)
{
	if (!scene->MatrixStack.empty())
	{
		scene->MatrixStack.push(scene->MatrixStack.top() * ToWorld);
	}
	else
	{
		scene->MatrixStack.push(ToWorld);
	}
	return SceneNode::PreRender(scene);
}

void TransformNode::PostRender(Scene* const scene)
{
	if (!scene->MatrixStack.empty())
	{
		scene->MatrixStack.pop();
	}
}

void TransformNode::SetTransform(const glm::mat4& toWorld)
{
	SetTransform(toWorld, glm::inverse(toWorld));
}

void TransformNode::SetTransform(const glm::mat4& toWorld, const glm::mat4& fromWorld)
{
	ToWorld = toWorld;
	FromWorld = fromWorld;
}
