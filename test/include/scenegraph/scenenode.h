#if ! defined(__SCENE_NODE__)
#define __SCENE_NODE__

#include <list>
#include <glm/glm.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

class Scene;

typedef boost::shared_ptr<class SceneNode> SceneNodePtr;
typedef std::list<SceneNodePtr> SceneNodeList;

class SceneNode : public boost::noncopyable
{
public:
	SceneNodeList				children;

	SceneNode();

	virtual ~SceneNode();

	void LoadNodeContent(Scene* const scene);

	void UpdateNode(Scene* const scene, float elapsedMS);

	void RenderNode(Scene* const scene);

	virtual void SetTransform(glm::mat4& toWorld) { }
	virtual void SetTransform(glm::mat4& toWorld, glm::mat4& fromWorld) { }

private:
	// These are the interfaces that concrete scene nodes should implement...

	// Implement this to initialise runtime content, load shaders, etc.
	virtual void LoadContent(Scene* const scene) { }

	// Implement this to modify the internal state of the node.
	virtual void Update(Scene* const scene, float elapsedMS) { }

	// Implememt this to perform actions just before rendering starts, e.g. visibility test.
	// Return false to prevent this _and_all_subsequent_ nodes from being rendered.
	virtual bool PreRender(Scene* const scene) { return true; }

	// Implement this to light up pixels, or otherwise perform "Magic". It's only called if
	// the node's PreRender() has returned true. 
	virtual void Render(Scene* const scene) { }

	// Implement this to perform actions after rendering, like save a texture out of the GPU
	// back into real memory. It's only called if the node's PreRender() has returned true.
	virtual void PostRender(Scene* const scene) { }
};

#endif // __SCENE_NODE__
