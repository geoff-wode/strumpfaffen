#if ! defined(__SCENE_H__)
#define __SCENE_H__

#include <map>
#include <stack>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <glm/glm.hpp>
#include <core/device.h>
#include <buffers/buffer.h>
#include <shaders/uniformblock.h>

#include <scenegraph/scenenode.h>
#include <scenegraph/camera.h>
#include <scenegraph/shadernode.h>
#include <scenegraph/transformnode.h>

namespace SceneGraph
{
	struct CommonShaderVarsBlock
	{
		glm::mat4 ModelMatrix;
		glm::mat4 ViewMatrix;
		glm::mat4 ProjectionMatrix;
		glm::mat4 ViewProjectionMatrix;
		glm::mat4 ModelViewProjectionMatrix;
		glm::vec3 CameraPos;
	};

	class Scene
	{
	public:
		typedef boost::shared_ptr<Scene> ScenePtr;
		static ScenePtr Create(Core::Device* const device);

		virtual ~Scene();

		boost::shared_ptr<class SceneNode> Root;
		std::stack<glm::mat4> MatrixStack;
		Core::Device* const Device;

		boost::shared_ptr<CameraNode> Camera;

		CommonShaderVarsBlock CommonShaderVars;

		typedef std::vector<Graphics::ShaderUniformBlock> UniformBlockList;
		UniformBlockList UniformBlocks;

	protected:
		Scene(Core::Device* device);
	};

	void LoadResources(Scene* const scene);
	void Update(Scene* const scene, unsigned int elapsedMS);
	void Render(Scene* const scene);
}

#endif // __SCENE_H__
