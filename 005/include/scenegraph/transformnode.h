#if ! defined(__TRANSFORMNODE__)
#define __TRANSFORMNODE__

#include <glm/glm.hpp>
#include <scenegraph/scenenode.h>

class TransformNode : public SceneNode
{
public:
	TransformNode(const glm::mat4& toWorld);
	TransformNode(const glm::mat4& toWorld, const glm::mat4& fromWorld);
	virtual ~TransformNode() { }

	virtual void SetTransform(const glm::mat4& toWorld);
	virtual void SetTransform(const glm::mat4& toWorld, const glm::mat4& fromWorld);

	const glm::mat4& ToWorld;
	const glm::mat4& FromWorld;

protected:
	virtual bool PreRenderNode(Scene* const scene);
	virtual void PostRenderNode(Scene* const scene);

private:
	glm::mat4 toWorld;
	glm::mat4 fromWorld;
};

#endif // __TRANSFORMNODE__
