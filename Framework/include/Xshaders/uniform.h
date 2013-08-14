// Declares the interface to a shader uniform variable.

#if ! defined(__UNIFORM_H__)
#define __UNIFORM_H__

#include <glm/glm.hpp>

namespace Graphics
{
	struct ShaderUniform
	{
		virtual void Set(float value) = 0;
		virtual void Set(const glm::vec2& value) = 0;
		virtual void Set(const glm::vec3& value) = 0;
		virtual void Set(const glm::vec4& value) = 0;
		virtual void Set(const glm::mat2& value) = 0;
		virtual void Set(const glm::mat3& value) = 0;
		virtual void Set(const glm::mat4& value) = 0;

		virtual float GetFloat() const = 0;
		virtual glm::vec2 GetVec2() const = 0;
		virtual glm::vec3 GetVec3() const = 0;
		virtual glm::vec4 GetVec4() const = 0;
		virtual glm::mat2 GetMat2() const = 0;
		virtual glm::mat3 GetMat3() const = 0;
		virtual glm::mat4 GetMat4() const = 0;
	};
}

#endif // __UNIFORM_H__
