#include <scenegraph/scene.h>
#include <scenegraph/camera.h>

using namespace SceneGraph;

//-------------------------------------------------------------------

CameraNodeClass::CameraNodeClass()
{
}

CameraNodeClass::~CameraNodeClass()
{
}

void CameraNodeClass::Update(unsigned int elapsedMS, Scene* const scene)
{
	// The inverse world-space matrix of a camera _IS_ the view matrix.
	scene->Device->ViewMatrix = FromWorld;
}
