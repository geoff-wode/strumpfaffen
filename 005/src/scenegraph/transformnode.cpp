#include <device.h>
#include <scenegraph/scene.h>
#include <scenegraph/transformnode.h>

//-------------------------------------------------------------

TransformNode::TransformNode(const glm::mat4& toWorld)
	: ToWorld(toWorld), FromWorld(fromWorld)
{
	SetTransform(toWorld);
}

TransformNode::TransformNode(const glm::mat4& toWorld, const glm::mat4& fromWorld)
	: ToWorld(toWorld), FromWorld(fromWorld)
{
	SetTransform(toWorld, fromWorld);
}

//-------------------------------------------------------------

void TransformNode::SetTransform(const glm::mat4& toWorld)
{
	SetTransform(toWorld, glm::inverse(toWorld));
}

//---------------------------------------------------------------------
void TransformNode::SetTransform(const glm::mat4& toWorld, const glm::mat4& fromWorld)
{
	this->toWorld = toWorld;
	this->fromWorld = fromWorld;
}

//---------------------------------------------------------------------
bool TransformNode::PreRenderNode(Scene* const scene)
{
	scene->matrixStack.Push(toWorld);
	Device::SetWorldMatrix(scene->matrixStack.Top());
	return true;
}

//---------------------------------------------------------------------
void TransformNode::PostRenderNode(Scene* const scene)
{
	scene->matrixStack.Pop();
	Device::SetWorldMatrix(scene->matrixStack.Top());
}

//---------------------------------------------------------------------
