#if ! defined(__UNIFORM_BLOCK_IMPL_H__)
#define __UNIFORM_BLOCK_IMPL_H__

#include <vector>
#include <memory>
#include <gl_loader/gl_loader.h>
#include <buffers/buffer.h>
#include <shaders/uniform.h>
#include "uniformimpl.h"

using namespace Graphics;

struct UniformBlockImpl
{
	std::string name;
	std::vector<UniformImpl> uniforms;
	std::vector<GLubyte> cache;
	Buffer buffer;
	GLuint bindingPoint;

	virtual const std::string& GetName() const;
	virtual ShaderUniform* const GetUniform(const std::string& name) const;
	void Apply();
};


#endif // __UNIFORM_BLOCK_IMPL_H__
