
#if ! defined(__SHADER_UNIFORM_IMPL__)
#define __SHADER_UNIFORM_IMPL__

#include "shader.h"
#include <gl_loader/gl_loader.h>

class ShaderUniformImpl : public ShaderUniform
{
public:
	ShaderUniformImpl(GLenum type, GLint location)
		: type(type), location(location), changed(false)
	{
	}

	virtual void Set(float value)            { CacheValue(&value, sizeof(value)); }
	virtual void Set(const glm::vec2& value) { CacheValue(&value, sizeof(value)); }
	virtual void Set(const glm::vec3& value) { CacheValue(&value, sizeof(value)); }
	virtual void Set(const glm::vec4& value) { CacheValue(&value, sizeof(value)); }
	virtual void Set(const glm::mat2& value) { CacheValue(&value, sizeof(value)); }
	virtual void Set(const glm::mat3& value) { CacheValue(&value, sizeof(value)); }
	virtual void Set(const glm::mat4& value) { CacheValue(&value, sizeof(value)); }

	virtual float GetFloat() const    { return *(float*)cache; }
	virtual glm::vec2 GetVec2() const { return *(glm::vec2*)cache; }
	virtual glm::vec3 GetVec3() const { return *(glm::vec3*)cache; }
	virtual glm::vec4 GetVec4() const { return *(glm::vec4*)cache; }
	virtual glm::mat2 GetMat2() const { return *(glm::mat2*)cache; }
	virtual glm::mat3 GetMat3() const { return *(glm::mat3*)cache; }
	virtual glm::mat4 GetMat4() const { return *(glm::mat4*)cache; }

	void CacheValue(const void* const value, size_t size)
	{
		if (0 != std::memcmp(cache, value, size))
		{
			std::memcpy(cache, value, size);
			changed = true;
		}
	}

	const GLenum type;
	const GLint location;
	bool changed;
	double cache[16];
};

#endif //__SHADER_UNIFORM_IMPL__
