#if ! defined(__SHADERS_H__)
#define __SHADERS_H__

#include <vector>
#include <string>
#include <gl/gl_loader.h>
#include <boost/scoped_ptr.hpp>
#include <boost/smart_ptr.hpp>
#include <stdint.h>
#include <glm/glm.hpp>

class Shader
{
public:
	static boost::shared_ptr<Shader> New();

	Shader();
	virtual ~Shader();

	bool Load(const std::string& vsSrc, const std::string& fsSrc);
	
	void Apply();

	void SetParam(size_t index, float value);
	void SetParam(size_t index, const glm::vec2& value);
	void SetParam(size_t index, const glm::vec3& value);
	void SetParam(size_t index, const glm::vec4& value);
	void SetParam(size_t index, const glm::mat2& value);
	void SetParam(size_t index, const glm::mat3& value);
	void SetParam(size_t index, const glm::mat4& value);

	size_t GetParamIndex(const char* name) const;

	void SetParam(const char* name, float value);
	void SetParam(const char* name, const glm::vec2& value);
	void SetParam(const char* name, const glm::vec3& value);
	void SetParam(const char* name, const glm::vec4& value);
	void SetParam(const char* name, const glm::mat2& value);
	void SetParam(const char* name, const glm::mat3& value);
	void SetParam(const char* name, const glm::mat4& value);

private:
	GLuint id;
	std::vector<struct ShaderParameter> params;
	void CacheParam(size_t index, const void* data, size_t size);
};

#endif // __SHADERS_H__
