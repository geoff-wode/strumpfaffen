#if ! defined(__UNIFORM_BLOCK_H__)
#define __UNIFORM_BLOCK_H__

#include <vector>
#include <memory>
#include <gl_loader/gl_loader.h>
#include <buffers/shaderuniformbuffer.h>
#include <shaders/uniform.h>
#include "uniformimpl.h"

struct UniformBlock
{
	std::vector<UniformImpl> uniforms;
	std::vector<GLubyte> cache;
	Buffers::ShaderUniformBuffer buffer;
	GLuint bindingPoint;

	void Apply()
	{
		size_t offset = 0;
		bool updateBuffer = false;
		for (size_t i = 0; i < uniforms.size(); ++i)
		{
			size_t dataSize = 0;
			switch (uniforms[i].type)
			{
			case GL_FLOAT:		  dataSize = sizeof(float); break;
			case GL_FLOAT_VEC2: dataSize = sizeof(glm::vec2); break;
			case GL_FLOAT_VEC3: dataSize = sizeof(glm::vec3); break;
			case GL_FLOAT_VEC4: dataSize = sizeof(glm::vec4); break;
			case GL_FLOAT_MAT2: dataSize = sizeof(glm::mat2); break;
			case GL_FLOAT_MAT3: dataSize = sizeof(glm::mat3); break;
			case GL_FLOAT_MAT4: dataSize = sizeof(glm::mat4); break;
			}
			if (uniforms[i].changed)
			{
				updateBuffer = true;
				memcpy(cache.data() + offset, uniforms[i].cache.data(), dataSize);
				uniforms[i].changed = false;
			}
			offset += dataSize;
		}
		if (updateBuffer)
		{
			buffer->Activate();
			buffer->SetData(cache.data(), cache.size(), 0);
			buffer->Deactivate();
		}
	}
};


#endif // __UNIFORM_BLOCK_H__
