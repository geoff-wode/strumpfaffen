#include <memory>
#include <util/debug.h>
#include <shaders/shaderparaminterface.h>
#include <gl_loader/gl_loader.h>
#include "blockparamimpl.h"

//--------------------------------------------------------------------------------

const std::string& ShaderBlockParam::GetName() const { return name; }

void ShaderBlockParam::Set(float value)						{ CacheValue(&value, sizeof(value)); }
void ShaderBlockParam::Set(const glm::vec2& value) { CacheValue(&value, sizeof(value)); }
void ShaderBlockParam::Set(const glm::vec3& value) { CacheValue(&value, sizeof(value)); }
void ShaderBlockParam::Set(const glm::vec4& value) { CacheValue(&value, sizeof(value)); }
void ShaderBlockParam::Set(const glm::mat2& value) { CacheValue(&value, sizeof(value)); }
void ShaderBlockParam::Set(const glm::mat3& value) { CacheValue(&value, sizeof(value)); }
void ShaderBlockParam::Set(const glm::mat4& value) { CacheValue(&value, sizeof(value)); }

//--------------------------------------------------------------------------------

float ShaderBlockParam::GetFloat() const		 { return *(float*)cache; }
glm::vec2 ShaderBlockParam::GetVec2() const { return *(glm::vec2*)cache; }
glm::vec3 ShaderBlockParam::GetVec3() const { return *(glm::vec3*)cache; }
glm::vec4 ShaderBlockParam::GetVec4() const { return *(glm::vec4*)cache; }
glm::mat2 ShaderBlockParam::GetMat2() const { return *(glm::mat2*)cache; }
glm::mat3 ShaderBlockParam::GetMat3() const { return *(glm::mat3*)cache; }
glm::mat4 ShaderBlockParam::GetMat4() const { return *(glm::mat4*)cache; }

//--------------------------------------------------------------------------------

void ShaderBlockParam::Apply()
{
	if (changed)
	{
		switch (type)
		{
		case GL_FLOAT:		  buffer->SetData(cache, sizeof(float), offset); break;
		case GL_FLOAT_VEC2: buffer->SetData(cache, sizeof(glm::vec2), offset); break;
		case GL_FLOAT_VEC3: buffer->SetData(cache, sizeof(glm::vec3), offset); break;
		case GL_FLOAT_VEC4: buffer->SetData(cache, sizeof(glm::vec4), offset); break;
		case GL_FLOAT_MAT2: buffer->SetData(cache, sizeof(glm::mat2), offset); break;
		case GL_FLOAT_MAT3: buffer->SetData(cache, sizeof(glm::mat3), offset); break;
		case GL_FLOAT_MAT4: buffer->SetData(cache, sizeof(glm::mat4), offset); break;
		default: LOG("unsupported shader param type\n"); break;
		}
		changed = false;
	}
}

//--------------------------------------------------------------------------------

void ShaderBlockParam::CacheValue(const void* const value, size_t size)
{
	if (0 != std::memcmp(cache, value, size))
	{
		std::memcpy(cache, value, size);
		changed = true;
	}
}
