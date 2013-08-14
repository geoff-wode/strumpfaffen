#if ! defined(__SCENEGRAPH_SCENENODE_H__)
#define __SCENEGRAPH_SCENENODE_H__

#include <list>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <glm/glm.hpp>

namespace Framework
{
	class Scene;

	typedef boost::shared_ptr<class SceneNode> SceneNodePtr;

	class SceneNode : public boost::noncopyable
	{
	public:
		virtual ~SceneNode();

		virtual void LoadResources(Scene* const scene);

		virtual void Update(Scene* const scene, unsigned int elapsedMS);
		virtual void RenderChildren(Scene* const scene);

		virtual bool PreRender(Scene* const scene);
		virtual void Render(Scene* const scene);
		virtual void PostRender(Scene* const scene);

		virtual void SetTransform(const glm::mat4& toWorld);
		virtual void SetTransform(const glm::mat4& toWorld, const glm::mat4& fromWorld);

		typedef std::list<SceneNodePtr> SceneNodeList;
		SceneNodeList childNodes;

	protected:
		SceneNode();
	};
}

#endif // __SCENEGRAPH_SCENENODE_H__
