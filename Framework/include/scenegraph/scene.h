#if ! defined(__SCENE_H__)
#define __SCENE_H__

#include <stack>
#include <boost/shared_ptr.hpp>
#include <glm/glm.hpp>
#include <core/device.h>

namespace SceneGraph
{

	typedef boost::shared_ptr<class SceneNodeClass> SceneNode;

	class Scene
	{
	public:
		Scene();
		virtual ~Scene();

		void Render();
		void LoadResources();
		void Update(unsigned int elapsedMS);

		SceneNode Root;
		std::stack<glm::mat4> MatrixStack;
		Core::Device* Device;
	};

}

#endif // __SCENE_H__
