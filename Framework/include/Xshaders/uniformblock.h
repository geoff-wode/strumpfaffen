#if ! defined(__UNIFORM_BLOCK_H__)
#define __UNIFORM_BLOCK_H__

#include <string>
#include <shaders/uniform.h>

namespace Graphics
{
	struct IUniformBlock
	{
		virtual const std::string& GetName() const = 0;
		virtual ShaderUniform* const GetUniform(const std::string& name) const = 0;
	};
}

#endif // __UNIFORM_BLOCK_H__
