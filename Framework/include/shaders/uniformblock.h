#if ! defined(__SHADER_UNIFORM_BLOCK_H__)
#define __SHADER_UNIFORM_BLOCK_H__

#include <boost/shared_ptr.hpp>
#include <glm/glm.hpp>
#include <shaders/uniform.h>
#include <buffers/buffer.h>

namespace Graphics
{
	struct ShaderUniformBlock
	{
		std::string name;
		unsigned int binding;
		std::vector<ShaderUniform> uniforms;
		Graphics::Buffer buffer;
	};
}

#endif // __SHADER_UNIFORM_BLOCK_H__
