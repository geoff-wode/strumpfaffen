#if ! defined(__SHADER__)
#define __SHADER__

#include <map>
#include <string>
#include <glm/glm.hpp>
#include <gl_loader/gl_loader.h>
#include <boost/shared_ptr.hpp>

class ShaderUniform
{
public:
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

typedef boost::shared_ptr<ShaderUniform> ShaderUniformPtr;
typedef std::map<std::string, ShaderUniformPtr> ShaderUniformMap;

class ShaderClass
{
public:
	ShaderClass(const std::string& name);
	~ShaderClass();

	bool Build();

	void Enable();

	void ApplyUniforms();

	ShaderUniformMap& Uniforms;

private:
	const std::string name;
	GLuint program;
	ShaderUniformMap uniforms;
};

typedef boost::shared_ptr<ShaderClass> Shader;

#endif // __SHADER__
