#include <shaders/shaderprogram.h>
#include <files.h>
#include <debug.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <gl_loader/gl_loader.h>

//----------------------------------------------------------------------------------

static GLuint CompileShader(GLenum type, const char* const src, size_t srcLen);
static void QueryShaderAttributes(GLuint program, ShaderAttributeMap& attributes);
static void QueryShaderUniforms(GLuint program, ShaderUniformMap& uniforms);

//----------------------------------------------------------------------------------

static const char CommonShaderSrc[] =
{
	"#version 330\n"
	"layout (std140) uniform CommonShaderVarsBlock\n"
	"{\n"
	"	mat4 ViewMatrix;\n"
	"	mat4 ProjectionMatrix;\n"
	"	mat4 ViewProjectionMatrix;\n"
	"	vec4 CameraPos;\n"
	"};\n"
	"\n"
	"layout(location = 0) in vec3 VertexPosition;\n"				// Used for single-precision floats
	"layout(location = 0) in vec3 VertexPositionHigh;\n"		// Used when emulating double-precision floats
	"layout(location = 1) in vec3 VertexPositionLow;\n"
	"layout(location = 2) in vec3 VertexNormal;\n"
	"layout(location = 3) in vec2 VertexTexCoord0;\n"
	"layout(location = 4) in vec3 VertexColour;\n"
	"\n"
	"#define PI 3.14159\n"
	"#define TWO_PI (PI * 2)\n"
	"#define PI_OVER_2 (PI * 0.5f)\n"
};

//----------------------------------------------------------------------------------

ShaderProgram::ShaderProgram(const std::string& name)
	: program(glCreateProgram())
{
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

	GLint linkedOK;
	glGetProgramiv(program, GL_LINK_STATUS, &linkedOK);
	if (linkedOK)
	{
		QueryShaderAttributes(program, attributes);
		QueryShaderUniforms(program, uniforms);
	}
	else
	{
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<GLchar> log(logLength+1);
		glGetProgramInfoLog(program, logLength, NULL, log.data());
		LOG("%s\n", log.data());
		exit(0);
	}
}

//----------------------------------------------------------------------------------
ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(program);
}

//----------------------------------------------------------------------------------
const ShaderAttributeMap* const ShaderProgram::GetAttributes() const
{
	return &attributes;
}

const ShaderUniformMap& ShaderProgram::GetUniforms() const
{
	return uniforms;
}

//----------------------------------------------------------------------------------
void ShaderProgram::Bind()
{
	glUseProgram(program);
}

void ShaderProgram::Apply()
{
	for (ShaderUniformMap::iterator i = uniforms.begin(); i != uniforms.end(); ++i)
	{
		i->second->Apply();
	}
}

//----------------------------------------------------------------------------------
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

static void QueryShaderAttributes(GLuint program, ShaderAttributeMap& attributes)
{
	GLint numAttrs;
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numAttrs);

	for (int i = 0; i < numAttrs; ++i)
	{
		char attrName[256] = { 0 };
		ShaderAttribute attr;

		glGetActiveAttrib(program, i, sizeof(attrName)-1, NULL, (GLint*)&attr.size, &attr.type, attrName);
		attr.location = glGetAttribLocation(program, attrName);

		attributes[attrName] = attr;
	}
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
			
			uniforms[uniformName] = boost::make_shared<ShaderUniform>(types[i], glGetUniformLocation(program, uniformName));
		}
	}
}
