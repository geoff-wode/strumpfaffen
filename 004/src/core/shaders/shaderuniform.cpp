#include <core/shaders/uniform.h>
#include <gl_loader/gl_loader.h>
#include <glm/ext.hpp>

using namespace core;

//--------------------------------------------------------------------------

static bool CacheValue(const void* const value, void* const cache, size_t size);

//--------------------------------------------------------------------------

ShaderUniform::ShaderUniform(GLenum type, unsigned int location)
	: Type(type), location(location), modified(false)
{
}

//--------------------------------------------------------------------------

void ShaderUniform::Set(float value) { modified |= CacheValue(this, &value, sizeof(value)); }
void ShaderUniform::Set(const glm::vec2& value) { modified |= CacheValue(glm::value_ptr(value), cache, sizeof(value)); }
void ShaderUniform::Set(const glm::vec3& value) { modified |= CacheValue(glm::value_ptr(value), cache, sizeof(value)); }
void ShaderUniform::Set(const glm::vec4& value) { modified |= CacheValue(glm::value_ptr(value), cache, sizeof(value)); }
void ShaderUniform::Set(const glm::mat2& value) { modified |= CacheValue(glm::value_ptr(value), cache, sizeof(value)); }
void ShaderUniform::Set(const glm::mat3& value) { modified |= CacheValue(glm::value_ptr(value), cache, sizeof(value)); }
void ShaderUniform::Set(const glm::mat4& value) { modified |= CacheValue(glm::value_ptr(value), cache, sizeof(value)); }

//--------------------------------------------------------------------------

float ShaderUniform::GetFloat() const { return *(float*)cache; }
glm::vec2 ShaderUniform::GetVec2() const { return *(glm::vec2*)cache; }
glm::vec3 ShaderUniform::GetVec3() const { return *(glm::vec3*)cache; }
glm::vec4 ShaderUniform::GetVec4() const { return *(glm::vec4*)cache; }
glm::mat2 ShaderUniform::GetMat2() const { return *(glm::mat2*)cache; }
glm::mat3 ShaderUniform::GetMat3() const { return *(glm::mat3*)cache; }
glm::mat4 ShaderUniform::GetMat4() const { return *(glm::mat4*)cache; }

//--------------------------------------------------------------------------

void ShaderUniform::Apply()
{
	if (modified)
	{
		switch (Type)
		{
		case GL_FLOAT:		  glUniform1fv(location, 1, (float*)cache); break;
		case GL_FLOAT_VEC2: glUniform2fv(location, 1, (float*)cache); break;
		case GL_FLOAT_VEC3: glUniform3fv(location, 1, (float*)cache); break;
		case GL_FLOAT_VEC4: glUniform4fv(location, 1, (float*)cache); break;
		case GL_FLOAT_MAT2: glUniformMatrix2fv(location, 1, GL_FALSE, (float*)cache); break;
		case GL_FLOAT_MAT3: glUniformMatrix3fv(location, 1, GL_FALSE, (float*)cache); break;
		case GL_FLOAT_MAT4: glUniformMatrix4fv(location, 1, GL_FALSE, (float*)cache); break;
		}
		modified = false;
	}
}

static bool CacheValue(const void* const value, void* const cache, size_t size)
{
	if (0 != memcmp(cache, value, size))
	{
		memcpy(cache, value, size);
		return true;
	}
	return false;
}
