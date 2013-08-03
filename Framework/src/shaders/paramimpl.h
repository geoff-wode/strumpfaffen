#if ! defined(__SHADER_PARAM_IMPL_H__)
#define __SHADER_PARAM_IMPL_H__

#include <string>
#include <glm/glm.hpp>
#include <gl_loader/gl_loader.h>
#include <shaders/shaderparaminterface.h>

class ShaderParamImpl : public IShaderParam
{
public:
	ShaderParamImpl(const std::string& name, GLenum type, GLuint location)
		: changed(false), name(name), type(type), location(location)
	{
	}

	virtual const std::string& GetName() const;

	virtual void Set(float value);
	virtual void Set(const glm::vec2& value);
	virtual void Set(const glm::vec3& value);
	virtual void Set(const glm::vec4& value);
	virtual void Set(const glm::mat2& value);
	virtual void Set(const glm::mat3& value);
	virtual void Set(const glm::mat4& value);

	virtual float GetFloat() const;
	virtual glm::vec2 GetVec2() const;
	virtual glm::vec3 GetVec3() const;
	virtual glm::vec4 GetVec4() const;
	virtual glm::mat2 GetMat2() const;
	virtual glm::mat3 GetMat3() const;
	virtual glm::mat4 GetMat4() const;

	virtual void Apply();

	void CacheValue(const void* const value, size_t size);

	bool changed;
	const std::string name;
	const GLenum type;
	const GLuint location;
	double cache[16];
};


#endif // __SHADER_PARAM_IMPL_H__
