#include <scenegraph/cameranode.h>
#include <scenegraph/scene.h>

CameraNode::CameraNode()
{
}

void CameraNode::Update(Scene* const scene, unsigned int elapsedMS)
{
	// Set the current view matrix to the _inverse_ of the camera's to-world matrix.
	scene->device.ViewMatrix = FromWorld;
}
