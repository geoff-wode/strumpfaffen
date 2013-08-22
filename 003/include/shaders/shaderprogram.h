// Ah, shaders! The life-blood of a modern graphics program...

#if ! defined(__SHADER_PROGRAM__)
#define __SHADER_PROGRAM__

#include <string>
#include <vector>
#include <gl_loader/gl_loader.h>
#include <shaders/shaderattribute.h>
#include <shaders/shaderuniform.h>

class ShaderProgram
{
public:
	ShaderProgram(const std::string& name);
	~ShaderProgram();

	bool Load();

	void Bind();
	void Apply();

	const ShaderAttributeMap* const GetAttributes() const;
	const ShaderUniformMap& GetUniforms() const;

private:
	const std::string name;
	GLuint program;
	ShaderAttributeMap attributes;
	ShaderUniformMap uniforms;
};

#endif // __SHADER_PROGRAM__
