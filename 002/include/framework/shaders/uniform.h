#if ! defined(__SHADER_UNIFORM_H__)
#define __SHADER_UNIFORM_H__

#include <memory>
#include <glm/glm.hpp>
#include <gl_loader/gl_loader.h>
#include <boost/shared_ptr.hpp>

namespace Framework
{
	class ShaderUniform
	{
	public:
		ShaderUniform(const std::string& name, GLenum type, GLuint location)
			: name(name), type(type), location(location), modified(false)
		{
			std::memset(cache, 0, sizeof(cache));
		}

		const std::string& GetName() const { return name; }

		void Set(float value) { CacheValue(&value, sizeof(value)); }
		void Set(const glm::vec2& value) { CacheValue(&value, sizeof(value)); }
		void Set(const glm::vec3& value) { CacheValue(&value, sizeof(value)); }
		void Set(const glm::vec4& value) { CacheValue(&value, sizeof(value)); }
		void Set(const glm::mat2& value) { CacheValue(&value, sizeof(value)); }
		void Set(const glm::mat3& value) { CacheValue(&value, sizeof(value)); }
		void Set(const glm::mat4& value) { CacheValue(&value, sizeof(value)); }

		float GetFloat() { return *(float*)cache; }
		glm::vec2 GetVec2() { return *(glm::vec2*)cache; }
		glm::vec3 GetVec3() { return *(glm::vec3*)cache; }
		glm::vec4 GetVec4() { return *(glm::vec4*)cache; }
		glm::mat2 GetMat2() { return *(glm::mat2*)cache; }
		glm::mat3 GetMat3() { return *(glm::mat3*)cache; }
		glm::mat4 GetMat4() { return *(glm::mat4*)cache; }

		void Apply()
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

	private:
		std::string name;
		GLenum type;
		GLuint location;
		bool modified;
		double cache[16];

		void CacheValue(const void* const value, size_t size)
		{
			if (0 != std::memcmp(cache, value, size))
			{
				std::memcpy(cache, value, size);
				modified = true;
			}
		}
	};

	typedef boost::shared_ptr<ShaderUniform> ShaderUniformPtr;
}

#endif // __SHADER_UNIFORM_H__
