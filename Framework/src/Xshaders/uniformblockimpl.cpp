#include "uniformblockimpl.h"

using namespace Graphics;

//--------------------------------------------------------------------------

const std::string& UniformBlockImpl::GetName() const
{
	return name;
}

ShaderUniform* const UniformBlockImpl::GetUniform(const std::string& name) const
{
	for (size_t i = 0; i < uniforms.size(); ++i)
	{
		if (name == uniforms[i].name) { return (ShaderUniform* const)&uniforms[i]; }
	}
	return NULL;
}

void UniformBlockImpl::Apply()
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
			memcpy(cache.data() + offset, uniforms[i].cache, dataSize);
			uniforms[i].changed = false;
		}
		offset += dataSize;
	}
	if (updateBuffer)
	{
		buffer->Enable();
		buffer->SetData(cache.data(), cache.size(), 0);
		buffer->Disable();
	}
}
