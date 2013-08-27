#include "debug.h"
#include "device.h"
#include <memory>
#include <vector>
#include "files.h"
#include "shaderuniformimpl.h"
#include "shadersemantic.h"
#include "shader.h"

//--------------------------------------------------------

static const char CommonShaderSrc[] =
{
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
	"	mat4 ViewProjectionMatrix;\n"
	"	mat4 WorldViewProjectionMatrix;\n"
	"};\n"
	"\n"
	"layout (location =  0) in vec4 Position		;\n"
	"layout (location =  0) in vec4 PositionLow	;\n"
	"layout (location =  1) in vec4 PositionHigh;\n"
	"layout (location =  2) in vec3 Normal			;\n"
	"layout (location =  3) in vec4 Colour			;\n"
	"layout (location =  4) in vec4 Texture0		;\n"
	"layout (location =  5) in vec4 Texture1		;\n"
	"layout (location =  6) in vec4 Texture2		;\n"
	"layout (location =  7) in vec4 Texture3		;\n"
	"\n"
};

//--------------------------------------------------------

static GLuint CompileShader(GLenum type, const char* const src, size_t srcLen);
static void QueryShaderUniforms(GLuint program, ShaderUniformMap& uniforms);
static void QueryShaderAttributes(GLuint program);
static const char* const GetAttribTypeName(unsigned int type);

//--------------------------------------------------------

ShaderClass::ShaderClass(const std::string& name)
	: name(name), Uniforms(uniforms)
{
}

ShaderClass::~ShaderClass()
{
	glDeleteProgram(program);
	program = 0;
}

bool ShaderClass::Build()
{
	program = glCreateProgram();

	std::vector<std::string> files;
	File::ListFiles(name, files);

	std::vector<GLuint> shaders;

	for (std::vector<std::string>::const_iterator i = files.begin(); i != files.end(); ++i)
	{
		std::vector<char> src;
		File::Load(*i, src);

		if (i->npos != i->find("vs.glsl")) { shaders.push_back(CompileShader(GL_VERTEX_SHADER, src.data(), src.size())); }
		if (i->npos != i->find("gs.glsl")) { shaders.push_back(CompileShader(GL_GEOMETRY_SHADER, src.data(), src.size())); }
		if (i->npos != i->find("fs.glsl")) { shaders.push_back(CompileShader(GL_FRAGMENT_SHADER, src.data(), src.size())); }
	}

	for (size_t i = 0; i < shaders.size(); ++i) { glAttachShader(program, shaders[i]); }

	glLinkProgram(program);

	for (size_t i = 0; i < shaders.size(); ++i)
	{
		glDetachShader(program, shaders[i]);
		glDeleteShader(shaders[i]);
	}

	LOG("shader: %s", name.c_str());

	GLint linkedOK;
	glGetProgramiv(program, GL_LINK_STATUS, &linkedOK);
	if (GL_TRUE == linkedOK)
	{
		QueryShaderAttributes(program);
		QueryShaderUniforms(program, uniforms);
	}
	else
	{
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<GLchar> log(logLength+1);
		glGetProgramInfoLog(program, logLength, NULL, log.data());
		LOG("\n%s\n", log.data());
	}

	return (GL_TRUE == linkedOK);
}

//--------------------------------------------------------
void ShaderClass::Enable()
{
	glUseProgram(program);
}

//--------------------------------------------------------

void ShaderClass::ApplyUniforms()
{	
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
	const char* const srcCodeParts[] = { CommonShaderSrc, src };
	const GLint srcCodeLengths[] = { sizeof(CommonShaderSrc), srcLen };

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

	char uniformName[256] = { 0 };

	for (int i = 0; i < numUniforms; ++i)
	{
		// not a named uniform block...
		glGetActiveUniformName(program, i, sizeof(uniformName)-1, NULL, uniformName);

		if (-1 == blockIndices[i])
		{			
			uniforms[uniformName] = boost::make_shared<ShaderUniformImpl>(types[i], glGetUniformLocation(program, uniformName));
		}
	}

	const GLuint uniformBlockIndex = glGetUniformBlockIndex(program, "CommonShaderVarsBlock");
	if (GL_INVALID_INDEX != uniformBlockIndex)
	{
		const GLuint uniformBufferBinding = Device::uniformBlockIndices["CommonShaderVarsBlock"];
		glUniformBlockBinding(program, uniformBlockIndex, uniformBufferBinding);

		GLint numBlockUniforms;
		glGetActiveUniformBlockiv(program, uniformBlockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &numBlockUniforms);

		LOG("\n  block 'CommonShaderVarsBlock' @ idx %d\n", uniformBlockIndex);

		std::vector<GLuint> blockVarIndices(numBlockUniforms);
		glGetActiveUniformBlockiv(program, uniformBlockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, (GLint*)blockVarIndices.data());

		std::vector<GLint> types(numBlockUniforms);
		std::vector<GLint> offsets(numBlockUniforms);
		glGetActiveUniformsiv(program, numBlockUniforms, blockVarIndices.data(), GL_UNIFORM_TYPE, types.data());
		glGetActiveUniformsiv(program, numBlockUniforms, blockVarIndices.data(), GL_UNIFORM_OFFSET, offsets.data());

		LOG("  block member name          offset  name\n"
				"-----------------------------------------\n");

		for (GLint i = 0; i < numBlockUniforms; ++i)
		{
			glGetActiveUniformName(program, blockVarIndices[i], sizeof(uniformName)-1, NULL, uniformName);
			LOG("  %-25.25s  %6d  %-20.20s\n", uniformName, offsets[i], GetAttribTypeName(types[i]));
		}
	}
}


//----------------------------------------------------------------------------------
static void QueryShaderAttributes(GLuint program)
{
	GLint numAttributes;
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numAttributes);

	LOG("\n  %d attribute(s):\n"
		"  name                       loc'n  #units  type\n"
		"------------------------------------------------\n", numAttributes);

	for (GLint i = 0; i < numAttributes; ++i)
	{
		char attrName[128];
		GLint numUnits;
		GLenum type;
		glGetActiveAttrib(program, i, sizeof(attrName)-1, NULL, &numUnits, &type, attrName);
		if (numUnits > 1)
		{
			for (GLint unit = 0; unit < numUnits; ++unit)
			{
				char unitName[256];
				sprintf(unitName, "%s[%d]", attrName, unit);
				const GLint location = glGetAttribLocation(program, unitName);
				LOG("  %-25.25s   %5d  %6d  %-20.20s\n", unitName, location, numUnits, GetAttribTypeName(type));
			}
		}
		else
		{
			const GLint location = glGetAttribLocation(program, attrName);
			LOG("  %-25.25s  %5d  %6d  %-20.20s\n", attrName, location, numUnits, GetAttribTypeName(type));
		}
	}
}

//----------------------------------------------------------------------------------
static const char* const GetAttribTypeName(unsigned int type)
{
	switch (type)
	{
	case GL_FLOAT						 : return "GL_FLOAT";
	case GL_FLOAT_VEC2			 : return "GL_FLOAT_VEC2";
	case GL_FLOAT_VEC3			 : return "GL_FLOAT_VEC3";
	case GL_FLOAT_VEC4			 : return "GL_FLOAT_VEC4";
	case GL_FLOAT_MAT2			 : return "GL_FLOAT_MAT2";
	case GL_FLOAT_MAT3			 : return "GL_FLOAT_MAT3";
	case GL_FLOAT_MAT4			 : return "GL_FLOAT_MAT4";
	case GL_FLOAT_MAT2x3		 : return "GL_FLOAT_MAT2x3";
	case GL_FLOAT_MAT2x4		 : return "GL_FLOAT_MAT2x4";
	case GL_FLOAT_MAT3x2		 : return "GL_FLOAT_MAT3x2";
	case GL_FLOAT_MAT3x4		 : return "GL_FLOAT_MAT3x4";
	case GL_FLOAT_MAT4x2		 : return "GL_FLOAT_MAT4x2";
	case GL_FLOAT_MAT4x3		 : return "GL_FLOAT_MAT4x3";
	case GL_INT							 : return "GL_INT	";
	case GL_INT_VEC2				 : return "GL_INT_VEC2";
	case GL_INT_VEC3				 : return "GL_INT_VEC3";
	case GL_INT_VEC4				 : return "GL_INT_VEC4";
	case GL_UNSIGNED_INT		 : return "GL_UNSIGNED_INT";
	case GL_UNSIGNED_INT_VEC2: return "GL_UNSIGNED_INT_VEC2";
	case GL_UNSIGNED_INT_VEC3: return "GL_UNSIGNED_INT_VEC3";
	case GL_UNSIGNED_INT_VEC4: return "GL_UNSIGNED_INT_VEC4";
	default: break;
	}
	return "<unknown>";
}
