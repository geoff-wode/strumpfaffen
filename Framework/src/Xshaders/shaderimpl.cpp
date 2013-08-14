#include "shaderimpl.h"

using namespace Graphics;

//-------------------------------------------------------------------


void ShaderImpl::EnumerateUniforms(std::vector<std::string>& uniformNames) const
{
	uniformNames.reserve(uniforms.size());
	for (size_t i = 0; i < uniforms.size(); ++i)
	{
		uniformNames.push_back(uniforms[i].name);
	}
}

void ShaderImpl::EnumerateUniformBlocks(std::vector<std::string>& blockNames) const
{
	blockNames.reserve(blocks.size());
	for (size_t i = 0; i < blocks.size(); ++i)
	{
		blockNames.push_back(uniforms[i].name);
	}
}

ShaderUniform* const ShaderImpl::GetUniform(const std::string& name) const
{
	for (size_t i = 0; i < uniforms.size(); ++i)
	{
		if (name == uniforms[i].name) { return (ShaderUniform* const)&uniforms[i]; }
	}
	return NULL;
}

IUniformBlock* const ShaderImpl::GetUniformBlock(const std::string& name) const
{
	for (size_t i = 0; i < blocks.size(); ++i)
	{
		if (name == blocks[i].name) { return (IUniformBlock* const)&blocks[i]; }
	}
	return NULL;
}

void ShaderImpl::Apply()
{
	glUseProgram(handle);
	for (size_t i = 0; i < uniforms.size(); ++i)
	{
		if (uniforms[i].changed)
		{
			switch (uniforms[i].type)
			{
			case GL_FLOAT:		  glUniform1fv(uniforms[i].location, 1, (float*)uniforms[i].cache); break;
			case GL_FLOAT_VEC2: glUniform2fv(uniforms[i].location, 1, (float*)uniforms[i].cache); break;
			case GL_FLOAT_VEC3: glUniform3fv(uniforms[i].location, 1, (float*)uniforms[i].cache); break;
			case GL_FLOAT_VEC4: glUniform4fv(uniforms[i].location, 1, (float*)uniforms[i].cache); break;
			case GL_FLOAT_MAT2: glUniformMatrix2fv(uniforms[i].location, 1, GL_FALSE, (float*)uniforms[i].cache); break;
			case GL_FLOAT_MAT3: glUniformMatrix3fv(uniforms[i].location, 1, GL_FALSE, (float*)uniforms[i].cache); break;
			case GL_FLOAT_MAT4: glUniformMatrix4fv(uniforms[i].location, 1, GL_FALSE, (float*)uniforms[i].cache); break;
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
