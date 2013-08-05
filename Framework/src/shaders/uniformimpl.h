// Implements a shader uniform.
// Shader uniform values are cached on the CPU to reduce cross-chip data transfers.

#if ! defined(__UNIFORM_IMLP_H__)
#define __UNIFORM_IMLP_H__

#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <string>
#include <gl_loader/gl_loader.h>
#include <shaders/shader.h>
#include <shaders/uniform.h>

struct UniformImpl : public Graphics::ShaderUniform
{
	bool changed;
	std::string name;
	GLenum type;
	GLuint location;
	std::vector<char> cache;

	virtual void Set(float value) { CacheValue(&value, sizeof(value)); }
	virtual void Set(const glm::vec2& value) { CacheValue(&value, sizeof(value)); }
	virtual void Set(const glm::vec3& value) { CacheValue(&value, sizeof(value)); }
	virtual void Set(const glm::vec4& value) { CacheValue(&value, sizeof(value)); }
	virtual void Set(const glm::mat2& value) { CacheValue(&value, sizeof(value)); }
	virtual void Set(const glm::mat3& value) { CacheValue(&value, sizeof(value)); }
	virtual void Set(const glm::mat4& value) { CacheValue(&value, sizeof(value)); }

	virtual float GetFloat() const { return *(float*)(cache.data()); }
	virtual glm::vec2 GetVec2() const { return *(glm::vec2*)(cache.data()); }
	virtual glm::vec3 GetVec3() const { return *(glm::vec3*)(cache.data()); }
	virtual glm::vec4 GetVec4() const { return *(glm::vec4*)(cache.data()); }
	virtual glm::mat2 GetMat2() const { return *(glm::mat2*)(cache.data()); }
	virtual glm::mat3 GetMat3() const { return *(glm::mat3*)(cache.data()); }
	virtual glm::mat4 GetMat4() const { return *(glm::mat4*)(cache.data()); }

	// If the given value is different to that already cached, then the cache
	// is updated.
	void CacheValue(const void* const value, size_t size)
	{
		if (0 != memcmp(value, &cache, size))
		{
			changed = true;
			memcpy(&cache, value, size);
		}
	}
};

#endif // __UNIFORM_IMLP_H__
