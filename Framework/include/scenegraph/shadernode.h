// Describes a shader in the scene graph.

#if ! defined(__SHADER_NODE_H__)
#define __SHADER_NODE_H__

#include <vector>
#include <string>
#include <scenegraph/scenenode.h>
#include <gl_loader/gl_loader.h>
#include <shaders/uniform.h>

namespace SceneGraph
{
	class ShaderNode : public SceneNode
	{
	public:
		static boost::shared_ptr<ShaderNode> Create(const std::string& filename);

		virtual ~ShaderNode();

		virtual void LoadResources(Scene* const scene);

		virtual void Render(Scene* const scene);

		const Graphics::ShaderUniformList& GetUniforms() const { return uniforms; }

	private:
		ShaderNode(const std::string& filename);
		std::string filename;
		GLuint program;
		Graphics::ShaderUniformList uniforms;
	};
}

#endif // __SHADER_NODE_H__
