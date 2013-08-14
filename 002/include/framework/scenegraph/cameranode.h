#if ! defined(__SCENEGRAPH_CAMERANODE_H__)
#define __SCENEGRAPH_CAMERANODE_H__

#include <framework/shaders/uniform.h>
#include <framework/scenegraph/transformnode.h>

namespace Framework
{
	class CameraNode : public TransformNode
	{
	public:
		CameraNode();

		virtual void Update(Scene* const scene, unsigned int elapsedMS);
	};
}

#endif // __SCENEGRAPH_CAMERANODE_H__
