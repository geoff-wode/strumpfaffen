#if ! defined(__SHADERS_H__)
#define __SHADERS_H__

#include <vector>
#include <string>
#include <gl/gl_loader.h>
#include <boost/smart_ptr.hpp>
#include <stdint.h>

struct ShaderParameterData
{
  bool dirty;       // whether the cache is out of step with the GPU side, forcing an upload on Shader::Apply if it is
  GLuint location;  // as given by the "layout (location = n)" attribute declaration
  GLenum type;      // the type of the data stored in the attribute (gleaned from the shader program itself)
  uint8_t data[16 * sizeof(double)]; // CPU cache for quick recall
  GLchar name[32];  // the name of the parameter (gleaned from the shader program itself)
};

struct ShaderData
{
	GLuint id;
	std::vector<ShaderParameterData> params;

	ShaderData() : id(-1) { }
	~ShaderData() { glDeleteProgram(id); }
};

typedef boost::shared_ptr<ShaderData> Shader;

bool NewShaderProgram(const std::string& vsSrc, const std::string& fsSrc, Shader* program);

#endif // __SHADERS_H__
