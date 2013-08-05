// Implements the details of a shader.

#if ! defined(__SHADER_IMPL_H__)
#define __SHADER_IMPL_H__

#include <vector>
#include <shaders/shader.h>
#include <shaders/uniform.h>
#include <util/debug.h>
#include "uniformblock.h"

using namespace Graphics;

struct ShaderImpl : public IShader
{
	GLuint handle;
	std::vector<UniformImpl> uniforms;
	std::vector<UniformBlock> blocks;

	virtual ShaderUniform* const GetUniform(const std::string& name) const
	{
		for (size_t i = 0; i < uniforms.size(); ++i)
		{
			if (name == uniforms[i].name) { return (ShaderUniform* const)&uniforms[i]; }
		}
		for (size_t i = 0; i < blocks.size(); ++i)
		{
			for (size_t j = 0; j < blocks[i].uniforms.size(); ++j)
			{
				if (name == blocks[i].uniforms[j].name) { return (ShaderUniform* const)&blocks[i].uniforms[j]; }
			}
		}
		return NULL;
	}

	virtual void Apply()
	{
		glUseProgram(handle);
		for (size_t i = 0; i < uniforms.size(); ++i)
		{
			if (uniforms[i].changed)
			{
				switch (uniforms[i].type)
				{
				case GL_FLOAT:		  glUniform1fv(uniforms[i].location, 1, (float*)uniforms[i].cache.data()); break;
				case GL_FLOAT_VEC2: glUniform2fv(uniforms[i].location, 1, (float*)uniforms[i].cache.data()); break;
				case GL_FLOAT_VEC3: glUniform3fv(uniforms[i].location, 1, (float*)uniforms[i].cache.data()); break;
				case GL_FLOAT_VEC4: glUniform4fv(uniforms[i].location, 1, (float*)uniforms[i].cache.data()); break;
				case GL_FLOAT_MAT2: glUniformMatrix2fv(uniforms[i].location, 1, GL_FALSE, (float*)uniforms[i].cache.data()); break;
				case GL_FLOAT_MAT3: glUniformMatrix3fv(uniforms[i].location, 1, GL_FALSE, (float*)uniforms[i].cache.data()); break;
				case GL_FLOAT_MAT4: glUniformMatrix4fv(uniforms[i].location, 1, GL_FALSE, (float*)uniforms[i].cache.data()); break;
				default: LOG("unsupported shader param type\n"); break;
				}
				uniforms[i].changed = false;
			}
		}
		for (size_t i = 0; i < blocks.size(); ++i)
		{
			blocks[i].Apply();
		}
		glUseProgram(0);
	}
};


#endif // __SHADER_IMPL_H__
