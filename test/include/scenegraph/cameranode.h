#if ! defined(__CAMERA_NODE__)
#define __CAMERA_NODE__

#include <scenegraph/transformnode.h>

// Controls the view transform of a 3D camera.
// It inherits from TransformNode to enable it to be moved under user control
// or attached to an object hierarchy within the scene graph to implement e.g.
// a chase camera.
class CameraNode : public TransformNode
{
public:
	CameraNode();

	virtual void Update(Scene* const scene, unsigned int elapsedMS);
};

#endif // __CAMERA_NODE__
