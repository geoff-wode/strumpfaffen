#if ! defined(__SHADER_H__)
#define __SHADER_H__

#include <string>
#include <boost/shared_ptr.hpp>
#include <shaders/shaderparameter.h>

namespace Shaders
{
	typedef boost::shared_ptr<class ShaderClass> Shader;

	class ShaderClass
	{
	public:
		static Shader Load(const std::string& filename);

		virtual ShaderParameter* const GetParameter(const std::string& name) const = 0;

		virtual void Apply() = 0;
	};
}

#endif // __SHADER_H__
