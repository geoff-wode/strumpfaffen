#if ! defined(__TRANSFORM_NODE_H__)
#define __TRANSFORM_NODE_H__

#include <glm/glm.hpp>
#include <scenegraph/scenenode.h>

namespace SceneGraph
{

	class TransformNodeClass : public SceneNodeClass
	{
	public:
		TransformNodeClass();
		virtual ~TransformNodeClass();

		virtual bool PreRender(Scene* const scene);
		virtual void PostRender(Scene* const scene);
		virtual void SetTransform(const glm::mat4& toWorld);
		virtual void SetTransform(const glm::mat4& toWorld, const glm::mat4& fromWorld);

		// Transform from object- to world-space.
		glm::mat4 ToWorld;

		// Transform from world- to object-space.
		glm::mat4 FromWorld;
	};

	typedef boost::shared_ptr<TransformNodeClass> TransformNode;
}

#endif // __TRANSFORM_NODE_H__
