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

	typedef struct ShaderParameter* Parameter;

	Shader();
	virtual ~Shader();

	bool Load(const std::string& vsSrc, const std::string& fsSrc);
	
	void Apply();

	Parameter GetParameter(const char* name) const;

	void SetParam(Parameter param, float value);
	void SetParam(Parameter param, const glm::vec2& value);
	void SetParam(Parameter param, const glm::vec3& value);
	void SetParam(Parameter param, const glm::vec4& value);
	void SetParam(Parameter param, const glm::mat2& value);
	void SetParam(Parameter param, const glm::mat3& value);
	void SetParam(Parameter param, const glm::mat4& value);

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
};

#endif // __SHADERS_H__
