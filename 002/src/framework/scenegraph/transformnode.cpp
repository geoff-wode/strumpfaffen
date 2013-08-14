#include <framework/scenegraph/scene.h>
#include <framework/scenegraph/transformnode.h>

using namespace Framework;

//----------------------------------------------------------------

TransformNode::TransformNode()
	: toWorld(1), fromWorld(1)
{
}

bool TransformNode::PreRender(Scene* const scene)
{
	scene->matrixStack.Push(toWorld);

	return true;
}

void TransformNode::PostRender(Scene* const scene)
{
	scene->matrixStack.Pop();
}

void TransformNode::SetTransform(const glm::mat4& toWorld)
{
	SetTransform(toWorld, glm::inverse(toWorld));
}

void TransformNode::SetTransform(const glm::mat4& toWorld, const glm::mat4& fromWorld)
{
	this->toWorld = toWorld;
	this->fromWorld = fromWorld;
}

const glm::mat4& TransformNode::ToWorld() const
{
	return toWorld;
}

const glm::mat4& TransformNode::FromWorld() const
{
	return fromWorld;
}
