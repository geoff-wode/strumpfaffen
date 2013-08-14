#if ! defined(__SHADER_PROGRAM__)
#define __SHADER_PROGRAM__

#include <string>
#include <map>
#include <gl_loader/gl_loader.h>
#include <shaders/shaderattribute.h>
#include <shaders/shaderuniform.h>

class ShaderProgram
{
public:
	ShaderProgram(const std::string& name);
	~ShaderProgram();

	void Use();

	const ShaderAttributeMap* const GetAttributes() const;
	const ShaderUniformMap* const GetUniforms() const;

private:
	GLuint program;
	ShaderAttributeMap attributes;
	ShaderUniformMap uniforms;
};

#endif // __SHADER_PROGRAM__
