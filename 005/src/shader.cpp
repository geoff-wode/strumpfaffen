#include <debug.h>
#include <memory>
#include <shader.h>
#include <vector>
#include <files.h>
#include "shaderuniformimpl.h"
#include "shadersemantics.h"
#include <boost/make_shared.hpp>
#include <boost/lexical_cast.hpp>

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
	"	mat4 WorldViewProjectionMatrix;\n"
	"	mat4 ViewProjectionMatrix;\n"
	"};\n"
	"\n"
	"layout (location =  0) in vec3 Position		;\n"
	"layout (location =  0) in vec3 PositionLow	;\n"
	"layout (location =  1) in vec3 PositionHigh;\n"
	"layout (location =  2) in vec4 Colour			;\n"
	"layout (location =  3) in vec3 Normal			;\n"
	"layout (location =  4) in vec2 TexCoord0		;\n"
	"layout (location =  5) in vec2 TexCoord1		;\n"
	"layout (location =  6) in vec2 TexCoord2		;\n"
	"layout (location =  7) in vec2 TexCoord3		;\n"
	"layout (location =  8) in vec2 TexCoord4		;\n"
	"layout (location =  9) in vec2 TexCoord5		;\n"
	"layout (location = 10) in vec2 TexCoord6		;\n"
	"layout (location = 11) in vec2 TexCoord7		;\n"
	"layout (location = 12) in vec2 TexCoord8		;\n"
	"layout (location = 13) in vec2 TexCoord9		;\n"
	"layout (location = 14) in vec2 TexCoord10	;\n"
	"layout (location = 15) in vec2 TexCoord11	;\n"
	"layout (location = 16) in vec2 TexCoord12	;\n"
	"layout (location = 17) in vec2 TexCoord13	;\n"
	"layout (location = 18) in vec2 TexCoord14	;\n"
	"layout (location = 19) in vec2 TexCoord15	;\n"
	"\n"
};

const std::string shaderSemantics[] =
{
	"Position",    
	"PositionLow", 
	"PositionHigh",
	"Colour",			 
	"Normal",			 
	"TexCoord0",	 
	"TexCoord1",	 
	"TexCoord2",	 
	"TexCoord3",	 
	"TexCoord4",	 
	"TexCoord5",	 
	"TexCoord6",	 
	"TexCoord7",	 
	"TexCoord8",	 
	"TexCoord9",	 
	"TexCoord10",	 
	"TexCoord11",	 
	"TexCoord12",	 
	"TexCoord13",	 
	"TexCoord14",	 
	"TexCoord15",	 
};
const size_t NumShaderSemantics = sizeof(shaderSemantics) / sizeof(shaderSemantics[0]);

//--------------------------------------------------------

static GLuint CompileShader(GLenum type, const char* const src, size_t srcLen);
static void QueryShaderUniforms(GLuint program, ShaderUniformMap& uniforms);
static void QueryShaderAttributes(GLuint program);

//--------------------------------------------------------

Shader::Shader(const std::string& name)
	: name(name), Uniforms(uniforms)
{
}

Shader::~Shader()
{
	glDeleteProgram(program);
	program = 0;
}

bool Shader::Build()
{
	program = glCreateProgram();

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

	for (size_t i = 0; i < shaders.size(); ++i) { glAttachShader(program, shaders[i]); }

	// Bind the "semantics" of attributes to their shader input counter-parts...
	//for (size_t i = 0; i < NumShaderSemantics; ++i) { glBindAttribLocation(program, i, shaderSemantics[i].c_str()); }

	glLinkProgram(program);

	for (size_t i = 0; i < shaders.size(); ++i)
	{
		glDetachShader(program, shaders[i]);
		glDeleteShader(shaders[i]);
	}

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
		LOG("%s\n", log.data());
	}

	return (GL_TRUE == linkedOK);
}

//--------------------------------------------------------
void Shader::Enable()
{
	glUseProgram(program);
}

//--------------------------------------------------------

void Shader::ApplyUniforms()
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


//----------------------------------------------------------------------------------
static void QueryShaderAttributes(GLuint program)
{
	GLint numAttributes;
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numAttributes);

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
				LOG("shader attrib %d: %s - %d x %s @ %d\n", i, unitName, numUnits, GetAttribTypeName(type), location);
			}
		}
		else
		{
			const GLint location = glGetAttribLocation(program, attrName);
			LOG("shader attrib %d: %s - %d x %s @ %d\n", i, attrName, numUnits, GetAttribTypeName(type), location);
		}
	}
}
