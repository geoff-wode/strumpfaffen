// Defines a single shader uniform parameter.

#if ! defined(__SHADER_PARAMETER_H__)
#define __SHADER_PARAMETER_H__

#include <string>
#include <glm/glm.hpp>
#include <gl_loader/gl_loader.h>

namespace Shaders
{

	class IShaderParameter
	{
	public:
		virtual const std::string& GetName() const = 0;

		virtual void Set(float value) = 0;
		virtual void Set(const glm::vec2& value) = 0;
		virtual void Set(const glm::vec3& value) = 0;
		virtual void Set(const glm::vec4& value) = 0;
		virtual void Set(const glm::mat2& value) = 0;
		virtual void Set(const glm::mat3& value) = 0;
		virtual void Set(const glm::mat4& value) = 0;

		virtual float GetFloat() = 0;
		virtual glm::vec2 GetVec2() = 0;
		virtual glm::vec3 GetVec3() = 0;
		virtual glm::vec4 GetVec4() = 0;
		virtual glm::mat2 GetMat2() = 0;
		virtual glm::mat3 GetMat3() = 0;
		virtual glm::mat4 GetMat4() = 0;

		virtual void Update() = 0;

	protected:
		~IShaderParameter() { }
	};

	typedef IShaderParameter* const ShaderParameter;

}

#endif // __SHADER_PARAMETER_H__
