#if ! defined(__SHADERS_H__)
#define __SHADERS_H__

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <shaders/shaderparaminterface.h>

namespace Graphics
{
	typedef boost::shared_ptr<class IShader> Shader;
	typedef std::vector<ShaderParamPtr> ShaderParamList;

	class IShader
	{
	public:
		static Shader Load(const std::string& name);

		virtual const ShaderParamList& GetParams() const = 0;
		virtual void Apply() = 0;

	protected:
		~IShader() { }
	};
}

#endif
