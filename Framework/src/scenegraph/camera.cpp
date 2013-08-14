#include <memory>
#include <scenegraph/scene.h>
#include <scenegraph/camera.h>
#include <glm/gtx/transform.hpp>

using namespace SceneGraph;

//-------------------------------------------------------------------

boost::shared_ptr<CameraNode> CameraNode::Create()
{
	boost::shared_ptr<CameraNode> node(new CameraNode());
	return node;
}

CameraNode::CameraNode()
{
}

CameraNode::~CameraNode()
{
}

void CameraNode::Update(Scene* const scene, unsigned int elapsedMS)
{
	// The inverse world-space matrix of a camera _IS_ the view matrix.
	scene->CommonShaderVars.ViewMatrix = FromWorld;

	// Simplest just to always re-calculate the perspective transform every frame...
	const float aspectRatio = scene->Device->GetViewport().GetAspectRatio();
	scene->CommonShaderVars.ProjectionMatrix = glm::perspective(FieldOfView, aspectRatio, NearClipDistance, FarClipDistance);
}
