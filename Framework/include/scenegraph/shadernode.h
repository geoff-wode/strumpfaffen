// Describes a shader in the scene graph.

#if ! defined(__SHADER_NODE_H__)
#define __SHADER_NODE_H__

#include <vector>
#include <string>
#include <scenegraph/scenenode.h>
#include <shaders/shader.h>

namespace SceneGraph
{
	class ShaderNodeClass : public SceneNodeClass
	{
	public:
		ShaderNodeClass(const std::string& filename);
		virtual ~ShaderNodeClass();

		virtual void LoadResources(Scene* const scene);
		virtual bool PreRender(Scene* const scene);

		Shaders::Shader GetShader() const;

	private:
		std::string filename;
		Shaders::Shader shader;
	};

	typedef boost::shared_ptr<ShaderNodeClass> ShaderNode;
}

#endif // __SHADER_NODE_H__
