#include <framework/scenegraph/scene.h>
#include <framework/scenegraph/cameranode.h>

using namespace Framework;

//------------------------------------------------------------------

CameraNode::CameraNode()
{
}

void CameraNode::Update(Scene* const scene, unsigned int elapsedMS)
{
	scene->shaderSceneGlobals.ViewMatrix = fromWorld;
}
