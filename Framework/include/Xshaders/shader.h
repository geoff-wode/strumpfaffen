// Declares the interface to a shader program.

#if ! defined(__SHADER_H__)
#define __SHADER_H__

#include <string>
#include <boost/shared_ptr.hpp>
#include <shaders/uniform.h>
#include <shaders/uniformblock.h>

namespace Graphics
{
	struct IShader
	{
		virtual void Apply() = 0;

		virtual void EnumerateUniforms(std::vector<std::string>& uniformNames) const = 0;
		virtual ShaderUniform* const GetUniform(const std::string& name) const = 0;

		virtual void EnumerateUniformBlocks(std::vector<std::string>& blockNames) const = 0;
		virtual IUniformBlock* const GetUniformBlock(const std::string& name) const = 0;
	};

	typedef boost::shared_ptr<IShader>	Shader;

	Shader LoadShader(const std::string& name);
}

#endif // __SHADER_H__
