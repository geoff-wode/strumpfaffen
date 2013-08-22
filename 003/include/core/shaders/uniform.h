#if ! defined(__CORE_SHADERS_UNIFORM__)
#define __CORE_SHADERS_UNIFORM__

#include <gl_loader/gl_loader.h>
#include <glm/glm.hpp>

namespace core
{
	class ShaderUniform
	{
	public:
		const GLenum Type;

		ShaderUniform(GLenum type, unsigned int location);

		void Apply();

		void Set(float value);
		void Set(const glm::vec2& value);
		void Set(const glm::vec3& value);
		void Set(const glm::vec4& value);
		void Set(const glm::mat2& value);
		void Set(const glm::mat3& value);
		void Set(const glm::mat4& value);

		float GetFloat() const;
		glm::vec2 GetVec2() const;
		glm::vec3 GetVec3() const;
		glm::vec4 GetVec4() const;
		glm::mat2 GetMat2() const;
		glm::mat3 GetMat3() const;
		glm::mat4 GetMat4() const;

	private:
		const unsigned int location;
		bool modified;
		double cache[16];
	};
}

#endif // __CORE_SHADERS_UNIFORM__
