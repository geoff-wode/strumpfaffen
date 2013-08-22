#include <core/shaders/shader.h>
#include <gl_loader/gl_loader.h>
#include <files.h>
#include <debug.h>
#include "shadercompilerdefines.h"

using namespace core;

//--------------------------------------------------------------------

typedef std::vector<ShaderUniform> ShaderUniformList;
typedef std::map<std::string, ShaderUniform*> ShaderUniformMap;

struct Shader::Impl
{
	GLuint program;
	ShaderUniformMap uniformMap;
	ShaderUniformList uniformList;
};

//--------------------------------------------------------------------

static GLuint CompileShader(GLenum type, const char* source);
static bool LinkShaders(const std::vector<GLuint>& shaders, GLuint* program);
static void QueryUniforms(GLuint program, ShaderUniformList& uniforms, ShaderUniformMap& map);
static void BindUniformBlock(GLuint program);

//--------------------------------------------------------------------

Shader::Shader(const std::string& name)
	: impl(new Impl())
{
	std::vector<std::string> files;
	File::ListFiles(name, files);

	std::vector<GLuint> shaders;

	for (std::vector<std::string>::const_iterator i = files.begin(); i != files.end(); ++i)
	{
		std::vector<char> src;
		File::Load(*i, src);

		if (i->npos != i->find("vs.glsl")) { shaders.push_back(CompileShader(GL_VERTEX_SHADER, src.data())); }
		if (i->npos != i->find("gs.glsl")) { shaders.push_back(CompileShader(GL_GEOMETRY_SHADER, src.data())); }
		if (i->npos != i->find("fs.glsl")) { shaders.push_back(CompileShader(GL_FRAGMENT_SHADER, src.data())); }
	}

	if (LinkShaders(shaders, &impl->program))
	{
		BindUniformBlock(impl->program);
		QueryUniforms(impl->program, impl->uniformList, impl->uniformMap);
	}
}

Shader::~Shader()
{
	glDeleteProgram(impl->program);
}

//--------------------------------------------------------------------

void Shader::Activate()
{
	glUseProgram(impl->program);
}

void Shader::Deactivate()
{
	glUseProgram(0);
}

//--------------------------------------------------------------------

ShaderUniform* const Shader::GetUniform(const std::string& name) const
{
	ShaderUniformMap::const_iterator iter = impl->uniformMap.find(name);
	if (impl->uniformMap.cend() != iter) { return iter->second; }
	return NULL;
}

//--------------------------------------------------------------------

void Shader::Update()
{
	for (size_t i = 0; i < impl->uniformList.size(); ++i)
	{
		impl->uniformList[i].Apply();
	}
}

//--------------------------------------------------------------------
static GLuint CompileShader(GLenum type, const char* source)
{
	const GLchar* sources[2] = { ShaderAutos, source };

	const GLuint shader = glCreateShader(type);
	glShaderSource(shader, 2, sources, NULL);
	glCompileShader(shader);

	GLint OK;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &OK);
	if (GL_TRUE != OK)
	{
		GLint logLength;
		glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<GLchar> log(logLength+1);
		glGetProgramInfoLog(shader, logLength, NULL, log.data());
		LOG("%s\n", log.data());
	}
	return shader;
}

//--------------------------------------------------------------------
static bool LinkShaders(const std::vector<GLuint>& shaders, GLuint* program)
{
	for (size_t i = 0; i < shaders.size(); ++i) { glAttachShader(*program, shaders[i]); }

	glLinkProgram(*program);

	for (size_t i = 0; i < shaders.size(); ++i)
	{
		glDetachShader(*program, shaders[i]);
		glDeleteShader(shaders[i]);
	}

	GLint linkedOK;
	glGetProgramiv(*program, GL_LINK_STATUS, &linkedOK);
	if (GL_TRUE != linkedOK)
	{
		GLint logLength;
		glGetProgramiv(*program, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<GLchar> log(logLength+1);
		glGetProgramInfoLog(*program, logLength, NULL, log.data());
		LOG("%s\n", log.data());
	}

	return (GL_TRUE == linkedOK);
}

//--------------------------------------------------------------------
static void QueryUniforms(GLuint program, ShaderUniformList& uniforms, ShaderUniformMap& map)
{
	GLint numUniforms;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUniforms);

	for (int i = 0; i < numUniforms; ++i)
	{
		char name[256];
		GLint uniformSize;
		GLenum uniformType;
		glGetActiveUniform(program, i, sizeof(name)-1, NULL, &uniformSize, &uniformType, name);

		uniforms.push_back(ShaderUniform(uniformType, glGetUniformLocation(program, name)));
		map[name] = &uniforms[uniforms.size()-1];
	}
}

//--------------------------------------------------------------------
static void BindUniformBlock(GLuint program)
{
	// If the shader actually uses any of the values in it, get the index into the
	// global uniforms block...
	const GLuint blockIndex = glGetUniformBlockIndex(program, "SharedUniformsBlock");
	if (GL_INVALID_INDEX != blockIndex)
	{
		glUniformBlockBinding(program, blockIndex, 0);
	}
}
