#if ! defined(__SHADER_H__)
#define __SHADER_H__

#include <string>
#include <map>
#include <list>
#include <glm/glm.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <gl_loader/gl_loader.h>

#include <terrene/h/shader/shaderattribute.h>
#include <terrene/h/shader/shaderparameter.h>

namespace Terrene
{
	class IShader
	{
	public:
		virtual ShaderParameter GetParameter(const std::string& name) const = 0;
		virtual ShaderAttribute GetAttribute(const std::string& name) const = 0;
		virtual void Activate() = 0;

	protected:
		~IShader() { }
	};

	typedef boost::shared_ptr<IShader> Shader;

	Shader LoadShader(const std::string& name);

}

#endif // __SHADER_H__
