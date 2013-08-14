#if ! defined(__SHADER_UNIFORM__)
#define __SHADER_UNIFORM__

#include <map>
#include <string>
#include <gl_loader/gl_loader.h>
#include <glm/glm.hpp>
#include <boost/shared_ptr.hpp>

class ShaderUniform
{
public:
	ShaderUniform(GLenum type, GLuint location);

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

	const GLenum type;
	const GLuint location;

private:
	bool modified;
	double cache[16];

	static void CacheValue(ShaderUniform* const uniform, const void* const value, size_t size);
};

typedef std::map<std::string, boost::shared_ptr<ShaderUniform>> ShaderUniformMap;

#endif // __SHADER_UNIFORM__
