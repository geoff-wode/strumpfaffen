#include <scenegraph/scene.h>
#include <scenegraph/transformnode.h>

//-----------------------------------------------------------

TransformNode::TransformNode()
	: ToWorld(toWorld), FromWorld(fromWorld),
		toWorld(glm::mat4(1)), fromWorld(glm::mat4(1))
{
}

//-----------------------------------------------------------

bool TransformNode::PreRender(Scene* const scene)
{
	scene->matrixStack.Push(toWorld);
	scene->device.WorldMatrix = scene->matrixStack.Peek();
	return true;
}

void TransformNode::PostRender(Scene* const scene)
{
	scene->matrixStack.Pop();
	scene->device.WorldMatrix = scene->matrixStack.Peek();
}

//-----------------------------------------------------------

void TransformNode::SetTransform(glm::mat4& toWorld)
{
	SetTransform(toWorld, glm::inverse(toWorld));
}

void TransformNode::SetTransform(glm::mat4& toWorld, glm::mat4& fromWorld)
{
	this->toWorld = toWorld;
	this->fromWorld = fromWorld;
}
