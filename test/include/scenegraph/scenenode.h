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
	const SceneNodePtr	parent;

	SceneNodeList				children;

	SceneNode(SceneNodePtr parent);

	virtual ~SceneNode();

	virtual void LoadContent(Scene* const scene);

	virtual void Update(Scene* const scene, unsigned int elapsedMS);

	// Allow for early-out on rendering segments of the scene graph by returning false...
	virtual bool PreRender(Scene* const scene);

	virtual void Render(Scene* const scene);
	virtual void PostRender(Scene* const scene);

	virtual void SetTransform(const glm::mat4& toWorld);
	virtual void SetTransform(const glm::mat4& toWorld, const glm::mat4& fromWorld);

	void RenderChildren(Scene* const scene);
};

#endif // __SCENE_NODE__
