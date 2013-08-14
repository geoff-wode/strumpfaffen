#if ! defined(__SCENEGRAPH_SCENE_H__)
#define __SCENEGRAPH_SCENE_H__

#include <boost/noncopyable.hpp>
#include <framework/scenegraph/scenenode.h>
#include <framework/scenegraph/cameranode.h>
#include <framework/scenegraph/meshnode.h>
#include <framework/scenegraph/shadernode.h>
#include <framework/scenegraph/transformnode.h>
#include <framework/scenegraph/matrixstack.h>
#include <framework/input/movementcontroller.h>
#include <framework/device.h>

namespace Framework
{
	class Scene : public boost::noncopyable
	{
	public:
		Scene(Device* const device);
		~Scene();

		void LoadResources();
		void Update();
		void Render();

		Device* const device;
		SceneNodePtr root;
		MatrixStack matrixStack;
		Input::MovementControllerPtr movementController;

		// Values shared by all shaders in the scene...
		struct ShaderSceneGlobals
		{
			glm::mat4 ViewMatrix;
			glm::mat4 ProjectionMatrix;
			glm::mat4 ViewProjectionMatrix;
			glm::vec4 CameraPosition;
		} shaderSceneGlobals;

	private:
		unsigned int lastTime;
		GLuint shaderSceneGlobalsBuffer;
	};
}

#endif // __SCENEGRAPH_SCENE_H__
