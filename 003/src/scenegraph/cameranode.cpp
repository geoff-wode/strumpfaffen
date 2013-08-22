#include <scenegraph/scene.h>
#include <scenegraph/cameranode.h>

CameraNode::CameraNode(const glm::mat4& toWorld)
	: TransformNode(toWorld)
{
}

// Ensure the current view transform is set.
void CameraNode::Update(Scene* const scene, float elapsedMS)
{
	scene->device->ViewMatrix = GetFromWorld();
}
