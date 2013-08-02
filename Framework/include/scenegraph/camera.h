#if ! defined(__CAMERA_H__)
#define __CAMERA_H__

#include <scenegraph/transformnode.h>

namespace SceneGraph
{
	class CameraNodeClass : public TransformNodeClass
	{
	public:
		CameraNodeClass();
		virtual ~CameraNodeClass();
		virtual void Update(unsigned int elapsedMS, Scene* const scene);
	};

	typedef boost::shared_ptr<CameraNodeClass> CameraNode;
}

#endif // __CAMERA_H__
