#include <scenegraph/transformnode.h>
#include <scenegraph/scene.h>

//--------------------------------------------------------------

TransformNode::TransformNode(const glm::mat4& toWorld)
	: toWorld(toWorld), fromWorld(glm::inverse(toWorld))
{
}

TransformNode::TransformNode(const glm::mat4& toWorld, const glm::mat4& fromWorld)
	: toWorld(toWorld), fromWorld(fromWorld)
{
}

// Ensure that the global "to-world" matrix is set to the scene's current top-of-stack.
bool TransformNode::PreRender(Scene* const scene)
{
	scene->matrixStack.Push(toWorld);
	scene->device->WorldMatrix = scene->matrixStack.Peek();
	return true;
}

// Restore the global "to-world" matrix to whatever it was before this node was rendered.
void TransformNode::PostRender(Scene* const scene)
{
	scene->matrixStack.Pop();
	scene->device->WorldMatrix = scene->matrixStack.Peek();
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

glm::mat4 TransformNode::GetToWorld() const { return toWorld; }
glm::mat4 TransformNode::GetFromWorld() const { return fromWorld; }
