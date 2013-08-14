#if ! defined(__CAMERA_H__)
#define __CAMERA_H__

#include <scenegraph/transformnode.h>

namespace SceneGraph
{
	class CameraNode : public TransformNode
	{
	public:
		static boost::shared_ptr<CameraNode> Create();

		virtual ~CameraNode();

		virtual void Update(Scene* const scene, unsigned int elapsedMS);

		float FieldOfView;
		float NearClipDistance;
		float FarClipDistance;

	protected:
		CameraNode();
	};
}

#endif // __CAMERA_H__
