#if ! defined(__SHADER_H__)
#define __SHADER_H__

#include <string>
#include <map>
#include <list>
#include <glm/glm.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <gl_loader/gl_loader.h>

namespace Terrene
{

	class SShaderAttribute
	{
	public:
		const std::string Name;
		const GLenum			Type;
		const int					Location;
		const size_t			ComponentCount;

		SShaderAttribute(const char* const name, GLenum type, int location, size_t componentCount)
			: Name(name), Type(type), Location(location), ComponentCount(componentCount)
		{
		}
	};

	typedef SShaderAttribute* ShaderAttribute;
	typedef std::list<ShaderAttribute> ShaderAttributeCollection;

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

	//-------------------------------------------------

	class IShader
	{
	public:
		virtual ShaderParameter GetParameter(const std::string& name) const = 0;
		virtual ShaderAttribute GetAttribute(const std::string& name) const = 0;
		virtual void Activate() = 0;

	protected:
		~IShader() { }
	};

	typedef boost::shared_ptr<IShader> Shader;

	Shader LoadShader(const std::string& name);

}

#endif // __SHADER_H__
