#include <gl_loader/gl_loader.h>
#include <shaders/shaderparameter.h>
#include "shaderparamimpl.h"
#include <memory>
#include <util/debug.h>

using namespace Shaders;

//---------------------------------------------------------------------------

ShaderParamImpl::ShaderParamImpl()
	: dirty(false)
{
}

ShaderParamImpl::~ShaderParamImpl()
{
}

const std::string& ShaderParamImpl::GetName() const { return name; }

void ShaderParamImpl::Set(float value) { CacheValue(&value, sizeof(value)); }
void ShaderParamImpl::Set(const glm::vec2& value) { CacheValue(&value, sizeof(value)); }
void ShaderParamImpl::Set(const glm::vec3& value) { CacheValue(&value, sizeof(value)); }
void ShaderParamImpl::Set(const glm::vec4& value) { CacheValue(&value, sizeof(value)); }
void ShaderParamImpl::Set(const glm::mat2& value) { CacheValue(&value, sizeof(value)); }
void ShaderParamImpl::Set(const glm::mat3& value) { CacheValue(&value, sizeof(value)); }
void ShaderParamImpl::Set(const glm::mat4& value) { CacheValue(&value, sizeof(value)); }

float ShaderParamImpl::GetFloat() { return *(float*)cache; }
glm::vec2 ShaderParamImpl::GetVec2() { return *(glm::vec2*)cache; }
glm::vec3 ShaderParamImpl::GetVec3() { return *(glm::vec3*)cache; }
glm::vec4 ShaderParamImpl::GetVec4() { return *(glm::vec4*)cache; }
glm::mat2 ShaderParamImpl::GetMat2() { return *(glm::mat2*)cache; }
glm::mat3 ShaderParamImpl::GetMat3() { return *(glm::mat3*)cache; }
glm::mat4 ShaderParamImpl::GetMat4() { return *(glm::mat4*)cache; }

void ShaderParamImpl::Update()
{
	if (dirty)
	{
		switch (type)
		{
		case GL_FLOAT:		  glUniform1fv(location, 1, (float*)cache); break;
		case GL_FLOAT_VEC2: glUniform2fv(location, 1, (float*)cache); break;
		case GL_FLOAT_VEC3: glUniform3fv(location, 1, (float*)cache); break;
		case GL_FLOAT_VEC4: glUniform4fv(location, 1, (float*)cache); break;
		case GL_FLOAT_MAT2: glUniformMatrix2fv(location, 1, GL_FALSE, (float*)cache); break;
		case GL_FLOAT_MAT3: glUniformMatrix3fv(location, 1, GL_FALSE, (float*)cache); break;
		case GL_FLOAT_MAT4: glUniformMatrix4fv(location, 1, GL_FALSE, (float*)cache); break;
		default: LOG("unsupported shader param type\n"); break;
		}
		dirty = false;
	}
}

void ShaderParamImpl::CacheValue(const void* const value, size_t sizeInBytes)
{
	if (0 != memcmp(value, cache, sizeInBytes))
	{
		dirty = true;
		memcpy(cache, value, sizeInBytes);
	}
}
