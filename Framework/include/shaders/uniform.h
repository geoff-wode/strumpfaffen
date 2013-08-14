#if ! defined(__SHADER_UNIFORM_H__)
#define __SHADER_UNIFORM_H__

#include <vector>
#include <glm/glm.hpp>
#include <gl_loader/gl_loader.h>

namespace Graphics
{
	struct ShaderUniform
	{
		ShaderUniform(const std::string& name, GLenum type, GLint location)
			: name(name), type(type), location(location), changed(false)
		{
		}

		const std::string name;
		const GLenum type;
		const GLint location;
		bool changed;
		double cache[16];

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
	};

	typedef std::vector<ShaderUniform> ShaderUniformList;
}

#endif // __SHADER_UNIFORM_H__
