#if ! defined(__SCENEGRAPH_TRANSFORMNODE_H__)
#define __SCENEGRAPH_TRANSFORMNODE_H__

#include <boost/shared_ptr.hpp>
#include <glm/glm.hpp>
#include <framework/scenegraph/scenenode.h>

namespace Framework
{
	class TransformNode : public SceneNode
	{
	public:
		TransformNode();

		virtual bool PreRender(Scene* const scene);
		virtual void PostRender(Scene* const scene);

		virtual void SetTransform(const glm::mat4& toWorld);
		virtual void SetTransform(const glm::mat4& toWorld, const glm::mat4& fromWorld);

		const glm::mat4& ToWorld() const;
		const glm::mat4& FromWorld() const;

	protected:
		glm::mat4 toWorld;
		glm::mat4 fromWorld;
	};
}

#endif // __SCENEGRAPH_TRANSFORMNODE_H__
