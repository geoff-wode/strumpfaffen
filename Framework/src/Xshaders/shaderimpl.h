// Implements the details of a shader.

#if ! defined(__SHADER_IMPL_H__)
#define __SHADER_IMPL_H__

#include <vector>
#include <shaders/shader.h>
#include <shaders/uniform.h>
#include <util/debug.h>
#include "uniformblockimpl.h"

using namespace Graphics;

struct ShaderImpl : public IShader
{
	GLuint handle;
	std::vector<UniformImpl> uniforms;
	std::vector<UniformBlockImpl> blocks;

	virtual void EnumerateUniforms(std::vector<std::string>& uniformNames) const;
	virtual void EnumerateUniformBlocks(std::vector<std::string>& blockNames) const;
	virtual ShaderUniform* const GetUniform(const std::string& name) const;
	virtual IUniformBlock* const GetUniformBlock(const std::string& name) const;
	virtual void Apply();
};


#endif // __SHADER_IMPL_H__
