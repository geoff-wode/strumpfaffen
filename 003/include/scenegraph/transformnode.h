#if ! defined(__TRANSFORM_NODE__)
#define __TRANSFORM_NODE__

#include <scenegraph/scenenode.h>

class TransformNode : public SceneNode
{
public:
	TransformNode(const glm::mat4& toWorld);
	TransformNode(const glm::mat4& toWorld, const glm::mat4& fromWorld);

	virtual bool PreRender(Scene* const scene);
	virtual void PostRender(Scene* const scene);

	virtual void SetTransform(const glm::mat4& toWorld);
	virtual void SetTransform(const glm::mat4& toWorld, const glm::mat4& fromWorld);

	glm::mat4 GetToWorld() const;
	glm::mat4 GetFromWorld() const;

private:
	glm::mat4 toWorld;
	glm::mat4 fromWorld;
};

#endif // __TRANSFORM_NODE__
