#include <debug.h>
#include <memory>
#include <shader.h>
#include <vector>
#include <files.h>
#include "shaderuniformimpl.h"
#include <boost/make_shared.hpp>

//--------------------------------------------------------

static const std::string CommonShaderSrc
	(
		"#version 330\n"
		"\n"
		"#define PI 3.14159\n"
		"#define TWO_PI (PI * 2)\n"
		"#define PI_OVER_2 (PI * 0.5f)\n"
		"\n"
		"layout (std140) uniform CommonShaderVarsBlock\n"
		"{\n"
		"	vec4 CameraPos;\n"
		" mat4 WorldMatrix;\n"
		"	mat4 ViewMatrix;\n"
		"	mat4 ProjectionMatrix;\n"
		" mat4 InverseWorldMatrix;\n"
		" mat4 InverseViewMatrix;\n"
		" mat4 WorldViewMatrix;\n"
		"	mat4 WorldViewProjectionMatrix;\n"
		"	mat4 ViewProjectionMatrix;\n"
		"};\n"
		"\n"
		"layout(location =  0) in vec3 " + ShaderSemantics::Position + ";\n"
		"layout(location =  0) in vec3 " + ShaderSemantics::PositionLow + ";\n"
		"layout(location =  1) in vec3 " + ShaderSemantics::PositionHigh + ";\n"
		"layout(location =  2) in vec3 " + ShaderSemantics::Colour + ";\n"
		"layout(location =  3) in vec3 " + ShaderSemantics::Normal + ";\n"
		"layout(location =  4) in vec2 " + ShaderSemantics::TexCoord0 + ";\n"
		"layout(location =  5) in vec2 " + ShaderSemantics::TexCoord1 + ";\n"
		"layout(location =  6) in vec2 " + ShaderSemantics::TexCoord2 + ";\n"
		"layout(location =  7) in vec2 " + ShaderSemantics::TexCoord3 + ";\n"
		"layout(location =  8) in vec2 " + ShaderSemantics::TexCoord4 + ";\n"
		"layout(location =  9) in vec2 " + ShaderSemantics::TexCoord5 + ";\n"
		"layout(location = 10) in vec2 " + ShaderSemantics::TexCoord6 + ";\n"
		"layout(location = 11) in vec2 " + ShaderSemantics::TexCoord7 + ";\n"
		"layout(location = 12) in vec2 " + ShaderSemantics::TexCoord8 + ";\n"
		"layout(location = 13) in vec2 " + ShaderSemantics::TexCoord9 + ";\n"
		"layout(location = 14) in vec2 " + ShaderSemantics::TexCoord10 + ";\n"
		"layout(location = 15) in vec2 " + ShaderSemantics::TexCoord11 + ";\n"
		"layout(location = 16) in vec2 " + ShaderSemantics::TexCoord12 + ";\n"
		"layout(location = 17) in vec2 " + ShaderSemantics::TexCoord13 + ";\n"
		"layout(location = 18) in vec2 " + ShaderSemantics::TexCoord14 + ";\n"
		"layout(location = 19) in vec2 " + ShaderSemantics::TexCoord15 + ";\n"
	);

//--------------------------------------------------------

static GLuint CompileShader(GLenum type, const char* const src, size_t srcLen);
static void QueryShaderUniforms(GLuint program, ShaderUniformMap& uniforms);

//--------------------------------------------------------

Shader::Shader(const std::string& name)
	: name(name), Uniforms(uniforms)
{
}

Shader::~Shader()
{
	glDeleteProgram(handle);
	handle = 0;
}

bool Shader::Build()
{
	std::vector<std::string> files;
	ListFiles(name, files);

	std::vector<GLuint> shaders;

	for (std::vector<std::string>::const_iterator i = files.begin(); i != files.end(); ++i)
	{
		std::vector<char> src;
		LoadFile(*i, src);

		if (i->npos != i->find("vs.glsl")) { shaders.push_back(CompileShader(GL_VERTEX_SHADER, src.data(), src.size())); }
		if (i->npos != i->find("gs.glsl")) { shaders.push_back(CompileShader(GL_GEOMETRY_SHADER, src.data(), src.size())); }
		if (i->npos != i->find("fs.glsl")) { shaders.push_back(CompileShader(GL_FRAGMENT_SHADER, src.data(), src.size())); }
	}

	for (size_t i = 0; i < shaders.size(); ++i) { glAttachShader(handle, shaders[i]); }

	glLinkProgram(handle);

	for (size_t i = 0; i < shaders.size(); ++i)
	{
		glDetachShader(handle, shaders[i]);
		glDeleteShader(shaders[i]);
	}

	GLint linkedOK;
	glGetProgramiv(handle, GL_LINK_STATUS, &linkedOK);
	if (GL_TRUE == linkedOK)
	{
		QueryShaderUniforms(handle, uniforms);
	}
	else
	{
		GLint logLength;
		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<GLchar> log(logLength+1);
		glGetProgramInfoLog(handle, logLength, NULL, log.data());
		LOG("%s\n", log.data());
	}

	return (GL_TRUE == linkedOK);
}

//--------------------------------------------------------

void Shader::Apply()
{
	glUseProgram(handle);
	
	for (ShaderUniformMap::const_iterator i = uniforms.cbegin(); i != uniforms.cend(); ++i)
	{
		ShaderUniformImpl* const impl = (ShaderUniformImpl* const)i->second.get();
		if (impl->changed)
		{
			switch (impl->type)
			{
			case GL_FLOAT:		  glUniform1fv(impl->location, 1, (float*)impl->cache); break;
			case GL_FLOAT_VEC2: glUniform2fv(impl->location, 1, (float*)impl->cache); break;
			case GL_FLOAT_VEC3: glUniform3fv(impl->location, 1, (float*)impl->cache); break;
			case GL_FLOAT_VEC4: glUniform4fv(impl->location, 1, (float*)impl->cache); break;
			case GL_FLOAT_MAT2: glUniformMatrix2fv(impl->location, 1, GL_FALSE, (float*)impl->cache); break;
			case GL_FLOAT_MAT3: glUniformMatrix3fv(impl->location, 1, GL_FALSE, (float*)impl->cache); break;
			case GL_FLOAT_MAT4: glUniformMatrix4fv(impl->location, 1, GL_FALSE, (float*)impl->cache); break;
			}
			impl->changed = false;
		}
	}
}

//--------------------------------------------------------
static GLuint CompileShader(GLenum type, const char* const src, size_t srcLen)
{
	const char* const srcCodeParts[] = { CommonShaderSrc.c_str(), src };
	const GLint srcCodeLengths[] = { CommonShaderSrc.length(), srcLen };

	const GLuint shader = glCreateShader(type);
	glShaderSource(shader, 2, srcCodeParts, srcCodeLengths);
	glCompileShader(shader);

	GLint compiledOK;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiledOK);

	if (!compiledOK)
	{
		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<char> log(logLength+1);
		glGetShaderInfoLog(shader, logLength, NULL, log.data());
		LOG("%s\n", log.data());
	}

	return shader;
}

//----------------------------------------------------------------------------------
static void QueryShaderUniforms(GLuint program, ShaderUniformMap& uniforms)
{
	GLint numUniforms;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUniforms);

	std::vector<GLuint> indices(numUniforms);
	for (GLint i = 0; i < numUniforms; ++i) { indices[i] = i; }

	std::vector<GLint> types(numUniforms);
	glGetActiveUniformsiv(program, numUniforms, indices.data(), GL_UNIFORM_TYPE, types.data());

	std::vector<GLint> blockIndices(numUniforms);
	glGetActiveUniformsiv(program, numUniforms, indices.data(), GL_UNIFORM_BLOCK_INDEX, blockIndices.data());

	for (int i = 0; i < numUniforms; ++i)
	{
		if (-1 == blockIndices[i])
		{
			// not a named uniform block...
			char uniformName[256] = { 0 };
			glGetActiveUniformName(program, i, sizeof(uniformName)-1, NULL, uniformName);
			
			uniforms[uniformName] = boost::make_shared<ShaderUniformImpl>(types[i], glGetUniformLocation(program, uniformName));
		}
	}
}
