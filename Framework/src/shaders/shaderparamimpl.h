#if ! defined(__SHADER_PARAM_IMPL_H__)
#define __SHADER_PARAM_IMPL_H__

#include <gl_loader/gl_loader.h>
#include <shaders/shaderparameter.h>

class ShaderParamImpl : public Shaders::ShaderParameter
{
public:
	ShaderParamImpl();

	virtual ~ShaderParamImpl();

	virtual const std::string& GetName() const;

	virtual void Set(float value);
	virtual void Set(const glm::vec2& value);
	virtual void Set(const glm::vec3& value);
	virtual void Set(const glm::vec4& value);
	virtual void Set(const glm::mat2& value);
	virtual void Set(const glm::mat3& value);
	virtual void Set(const glm::mat4& value);

	virtual float GetFloat();
	virtual glm::vec2 GetVec2();
	virtual glm::vec3 GetVec3();
	virtual glm::vec4 GetVec4();
	virtual glm::mat2 GetMat2();
	virtual glm::mat3 GetMat3();
	virtual glm::mat4 GetMat4();

	virtual void Update();

	void CacheValue(const void* const value, size_t sizeInBytes);

	bool dirty;				// whether the parameter's value has changed since the last update
	std::string name;
	GLuint location;	// shader location of the parameter
	GLenum type;
	double cache[16];	// cache large enough for a 4x4 matrix of double-precision floats
};

#endif // __SHADER_PARAM_IMPL_H__
