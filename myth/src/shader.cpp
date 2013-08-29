#include <shader.h>
#include <files.h>
#include <debug.h>
#include <vector>

//--------------------------------------------------

static std::vector<GLuint> Compile(const std::vector<std::string>& shaderNames);
static GLuint CompileShader(const char* const code, int codeLength, GLenum type);
static bool LinkProgram(const std::vector<GLuint> shaders, GLuint programHandle);
static void QueryShaderAttributes(GLuint program);

//--------------------------------------------------

static const char CommonShaderCode[] =
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
};

//--------------------------------------------------

Shader::Shader(const std::string& name)
  : programHandle(glCreateProgram())
{
  std::vector<std::string> shaderNames;
  ListFiles(name, shaderNames);
  std::vector<GLuint> shaders = Compile(shaderNames);
	LOG("loading shader: %s", name.c_str());
  if (LinkProgram(shaders, programHandle))
  {
    LOG(" - built ok\n");
    QueryShaderAttributes(programHandle);
  }
}

Shader::~Shader()
{
  glDeleteProgram(programHandle);
}

//--------------------------------------------------

void Shader::Use()
{
  glUseProgram(programHandle);
}

//--------------------------------------------------

GLint Shader::GetAttributeIndex(const std::string& name)
{
  return glGetAttribLocation(programHandle, name.c_str());
}

//--------------------------------------------------

static std::vector<GLuint> Compile(const std::vector<std::string>& shaderNames)
{
  std::vector<GLuint> shaders(shaderNames.size());

  std::vector<char> srcCode;
  for (size_t i = 0; i < shaderNames.size(); ++i)
  {
    LoadFile(shaderNames[i], srcCode);
    if (std::string::npos != shaderNames[i].find("vs.glsl"))
    {
      shaders[i] = CompileShader(srcCode.data(), srcCode.size(), GL_VERTEX_SHADER);
    }
    else if (std::string::npos != shaderNames[i].find("fs.glsl"))
    {
      shaders[i] = CompileShader(srcCode.data(), srcCode.size(), GL_FRAGMENT_SHADER);
    }
    else if (std::string::npos != shaderNames[i].find("gs.glsl"))
    {
      shaders[i] = CompileShader(srcCode.data(), srcCode.size(), GL_GEOMETRY_SHADER);
    }
  }
  return shaders;
}

//--------------------------------------------------

static GLuint CompileShader(const char* const code, int codeLength, GLenum type)
{
  const char* source[] = { CommonShaderCode, code };
  const GLint lengths[] = { sizeof(CommonShaderCode), codeLength };
	const GLuint shader = glCreateShader(type);
	glShaderSource(shader, 2, source, lengths);
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

//--------------------------------------------------

static bool LinkProgram(const std::vector<GLuint> shaders, GLuint programHandle)
{
	for (size_t i = 0; i < shaders.size(); ++i) { glAttachShader(programHandle, shaders[i]); }

	glLinkProgram(programHandle);

	for (size_t i = 0; i < shaders.size(); ++i)
	{
		glDetachShader(programHandle, shaders[i]);
		glDeleteShader(shaders[i]);
	}

	GLint linkedOK;
	glGetProgramiv(programHandle, GL_LINK_STATUS, &linkedOK);
	if (GL_TRUE != linkedOK)
	{
		GLint logLength;
		glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<GLchar> log(logLength+1);
		glGetProgramInfoLog(programHandle, logLength, NULL, log.data());
		LOG("\n%s\n", log.data());
	}
  return (GL_TRUE == linkedOK);
}

//--------------------------------------------------
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
		LOG("  %s\n", attrName);
	}
}
