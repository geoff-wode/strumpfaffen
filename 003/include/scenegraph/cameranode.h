#if ! defined(__CAMERA_NODE__)
#define __CAMERA_NODE__

#include <scenegraph/transformnode.h>

class CameraNode : public TransformNode
{
public:
	CameraNode(const glm::mat4& toWorld);

	virtual void Update(Scene* const scene, float elapsedMS);
};

#endif // __CAMERA_NODE__
