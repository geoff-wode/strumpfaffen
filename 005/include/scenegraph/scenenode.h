#if ! defined(__SCENENODE__)
#define __SCENENODE__

#include <list>
#include <glm/glm.hpp>
#include <boost/shared_ptr.hpp>

typedef boost::shared_ptr<class SceneNode> SceneNodePtr;

class Scene;

class SceneNode
{
public:
	typedef std::list<SceneNodePtr> List;
	List children;

	SceneNode() { }
	virtual ~SceneNode();

	// Give the node an opportunity to load "expensive" resources (textures, shaders, etc.).
	void LoadContent(Scene* const scene);

	// Update the current state of the node.
	void UpdateState(Scene* const scene, unsigned int elapsedMS);

	// Light up some pixels.
	void Render(Scene* const scene);

	// The interface for storing the "frame of reference" of a node.
	// In the base class, it does very little...
	virtual void SetTransform(const glm::mat4& toWorld) { }
	virtual void SetTransform(const glm::mat4& toWorld, const glm::mat4& fromWorld) { }

protected:
	virtual void LoadNodeContent(Scene* const scene) { }
	virtual void UpdateNodeState(Scene* const scene, unsigned int elapsedMS) { }
	virtual bool PreRenderNode(Scene* const scene) { return true; }
	virtual void RenderNode(Scene* const scene) { }
	virtual void PostRenderNode(Scene* const scene) { }
};

#endif // __SCENENODE__
