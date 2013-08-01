#if ! defined(__SHADER_H__)
#define __SHADER_H__

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <shaders/shadervertexattribute.h>
#include <shaders/shaderparameter.h>

namespace Shaders
{

	typedef std::vector<ShaderVertexAttribute>	ShaderVertexAttributeList;
	typedef std::vector<IShaderParameter>				ShaderParameterList;

	class IShader
	{
	public:
		virtual const ShaderVertexAttribute* const GetVertexAttribute(const std::string& name) const = 0;

		virtual IShaderParameter* const GetParameter(const std::string& name) const = 0;

		virtual void Activate() = 0;

	protected:
		~IShader() { }
	};

	typedef boost::shared_ptr<IShader> Shader;
	Shader LoadShader(const std::string& name);

}

#endif // __SHADER_H__
