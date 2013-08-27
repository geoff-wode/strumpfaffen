#if ! defined(__TRANSFORM_NODE__)
#define __TRANSFORM_NODE__

#include <scenegraph/scenenode.h>

class TransformNode : public SceneNode
{
public:
	const glm::mat4& ToWorld;
	const glm::mat4& FromWorld;

	TransformNode(SceneNodePtr parent);

	// Creates a new model transform and loads it into the WorldMatrix shader variable.
	virtual bool PreRender(Scene* const scene);

	// Restores the WorldMatrix shader value to its value before this node was rendered.
	virtual void PostRender(Scene* const scene);

	virtual void SetTransform(const glm::mat4& toWorld);
	virtual void SetTransform(const glm::mat4& toWorld, const glm::mat4& fromWorld);

private:
	glm::mat4 toWorld;
	glm::mat4 fromWorld;
};

#endif // __TRANSFORM_NODE__
