#if ! defined(__SCENE_NODE_H__)
#define __SCENE_NODE_H__

#include <boost/shared_ptr.hpp>
#include <list>
#include <glm/glm.hpp>

namespace SceneGraph
{
	class Scene;

	class SceneNode
	{
	public:
		virtual ~SceneNode();

		// Causes all child nodes to be rendered.
		virtual void RenderChildren(Scene* const scene);

		// Enables this and all child nodes to (re-)load transient resources
		// such as model meshes, shaders, etc. if they are lost from memory.
		virtual void LoadResources(Scene* const scene);

		// Enables this and all child nodes to update internal state.
		virtual void Update(Scene* const scene, unsigned int elapsedMS);

		// Perform tasks which must complete before the node is rendered, such
		// as visibility tests, etc.
		// Return true (default) if the node should be rendered, otherwise false.
		virtual bool PreRender(Scene* const scene);

		// Render the node. Will only be called if PreRender returned true.
		virtual void Render(Scene* const scene);

		// Clean up/undo any actions taken in PreRender. Will only be called if
		// PreRender returned true.
		virtual void PostRender(Scene* const scene);

		virtual void SetTransform(const glm::mat4& toWorld);
		virtual void SetTransform(const glm::mat4& toWorld, const glm::mat4& fromWorld);

		typedef boost::shared_ptr<SceneNode> SceneNodePtr;
		typedef std::list<SceneNodePtr> NodeList;
		NodeList	Children;

	protected:
		SceneNode();
	};

}

#endif // __SCENE_NODE_H__
