#if ! defined(__SCENE_NODE__)
#define __SCENE_NODE__

#include <list>
#include <glm/glm.hpp>
#include <boost/shared_ptr.hpp>

class Scene;

typedef std::list<boost::shared_ptr<class SceneNode>> SceneNodeList;

class SceneNode
{
public:
	SceneNodeList children;

	virtual ~SceneNode();

	virtual void Load(Scene* const scene);

	virtual void RenderChildren(Scene* const scene);

	virtual void Update(Scene* const scene, float elapsedMS);

	virtual bool PreRender(Scene* const scene);
	virtual void Render(Scene* const scene);
	virtual void PostRender(Scene* const scene);

	virtual void SetTransform(const glm::mat4& toWorld);
	virtual void SetTransform(const glm::mat4& toWorld, const glm::mat4& fromWorld);

protected:
	SceneNode();
};

#endif // __SCENE_NODE__
