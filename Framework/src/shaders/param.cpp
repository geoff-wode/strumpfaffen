#include <memory>
#include <util/debug.h>
#include <shaders/shaderparaminterface.h>
#include <gl_loader/gl_loader.h>
#include "paramimpl.h"

//--------------------------------------------------------------------------------

const std::string& ShaderParamImpl::GetName() const { return name; }

void ShaderParamImpl::Set(float value)						{ CacheValue(&value, sizeof(value)); }
void ShaderParamImpl::Set(const glm::vec2& value) { CacheValue(&value, sizeof(value)); }
void ShaderParamImpl::Set(const glm::vec3& value) { CacheValue(&value, sizeof(value)); }
void ShaderParamImpl::Set(const glm::vec4& value) { CacheValue(&value, sizeof(value)); }
void ShaderParamImpl::Set(const glm::mat2& value) { CacheValue(&value, sizeof(value)); }
void ShaderParamImpl::Set(const glm::mat3& value) { CacheValue(&value, sizeof(value)); }
void ShaderParamImpl::Set(const glm::mat4& value) { CacheValue(&value, sizeof(value)); }

//--------------------------------------------------------------------------------

float ShaderParamImpl::GetFloat() const		 { return *(float*)cache; }
glm::vec2 ShaderParamImpl::GetVec2() const { return *(glm::vec2*)cache; }
glm::vec3 ShaderParamImpl::GetVec3() const { return *(glm::vec3*)cache; }
glm::vec4 ShaderParamImpl::GetVec4() const { return *(glm::vec4*)cache; }
glm::mat2 ShaderParamImpl::GetMat2() const { return *(glm::mat2*)cache; }
glm::mat3 ShaderParamImpl::GetMat3() const { return *(glm::mat3*)cache; }
glm::mat4 ShaderParamImpl::GetMat4() const { return *(glm::mat4*)cache; }

//--------------------------------------------------------------------------------

void ShaderParamImpl::Apply()
{
	if (changed)
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
		changed = false;
	}
}

//--------------------------------------------------------------------------------

void ShaderParamImpl::CacheValue(const void* const value, size_t size)
{
	if (0 != std::memcmp(cache, value, size))
	{
		std::memcpy(cache, value, size);
		changed = true;
	}
}
