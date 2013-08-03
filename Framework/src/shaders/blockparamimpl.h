#if ! defined(__SHADER_BLOCK_PARAM_IMPL_H__)
#define __SHADER_BLOCK_PARAM_IMPL_H__

#include <string>
#include <glm/glm.hpp>
#include <gl_loader/gl_loader.h>
#include <shaders/shaderparam.h>
#include <buffers/shaderuniformbuffer.h>

class ShaderBlockParam : public IShaderParam
{
public:
	ShaderBlockParam(const std::string& name, GLenum type, Buffers::ShaderUniformBuffer buffer, size_t offset)
		: changed(false), name(name), type(type), offset(offset), buffer(buffer)
	{
	}

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

	bool changed;
	const std::string name;
	const GLenum type;
	const size_t offset;
	const Buffers::ShaderUniformBuffer buffer;
	double cache[16];
};

#endif // __SHADER_BLOCK_PARAM_IMPL_H__
