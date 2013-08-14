#if ! defined(__SHADER_H__)
#define __SHADER_H__

#include <map>
#include <vector>
#include <string>
#include <gl_loader/gl_loader.h>
#include <boost/shared_ptr.hpp>
#include <framework/shaders/uniform.h>

namespace Framework
{
	typedef std::map<std::string, ShaderUniformPtr> ShaderUniformMap;

	struct ShaderVertexAttribute
	{
		std::string name;
		GLenum type;
		GLuint location;
		GLuint components;
	};
	typedef std::vector<ShaderVertexAttribute> ShaderVertexAttributeList;

	class Shader
	{
	public:
		Shader(const std::string& name, class Device* const device);
		virtual ~Shader();

		const ShaderUniformMap& GetUniforms() const { return uniforms; }

		void Enable();
		void Apply();
		void Disable();

	protected:
		GLuint program;
		ShaderUniformMap uniforms;

		ShaderVertexAttributeList vertexAttrs;
	};

	typedef boost::shared_ptr<Shader> ShaderPtr;
}

#endif // __SHADER_H__
