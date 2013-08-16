#include <shaders/shaderuniform.h>
#include <memory>
#include <glm/ext.hpp>

ShaderUniform::ShaderUniform(GLenum type, GLuint location)
	: type(type), location(location), modified(false)
{
}

void ShaderUniform::Set(float value) { CacheValue(this, &value, sizeof(value)); }
void ShaderUniform::Set(const glm::vec2& value) { CacheValue(this, glm::value_ptr(value), sizeof(value)); }
void ShaderUniform::Set(const glm::vec3& value) { CacheValue(this, glm::value_ptr(value), sizeof(value)); }
void ShaderUniform::Set(const glm::vec4& value) { CacheValue(this, glm::value_ptr(value), sizeof(value)); }
void ShaderUniform::Set(const glm::mat2& value) { CacheValue(this, glm::value_ptr(value), sizeof(value)); }
void ShaderUniform::Set(const glm::mat3& value) { CacheValue(this, glm::value_ptr(value), sizeof(value)); }
void ShaderUniform::Set(const glm::mat4& value) { CacheValue(this, glm::value_ptr(value), sizeof(value)); }

float ShaderUniform::GetFloat() const { return *(float*)cache; }
glm::vec2 ShaderUniform::GetVec2() const { return *(glm::vec2*)cache; }
glm::vec3 ShaderUniform::GetVec3() const { return *(glm::vec3*)cache; }
glm::vec4 ShaderUniform::GetVec4() const { return *(glm::vec4*)cache; }
glm::mat2 ShaderUniform::GetMat2() const { return *(glm::mat2*)cache; }
glm::mat3 ShaderUniform::GetMat3() const { return *(glm::mat3*)cache; }
glm::mat4 ShaderUniform::GetMat4() const { return *(glm::mat4*)cache; }

void ShaderUniform::Apply()
{
	if (modified)
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
		}
		modified = false;
	}
}

void ShaderUniform::CacheValue(ShaderUniform* const uniform, const void* const value, size_t size)
{
	if (0 != memcmp(uniform->cache, value, size))
	{
		memcpy(uniform->cache, value, size);
		uniform->modified = true;
	}
}
