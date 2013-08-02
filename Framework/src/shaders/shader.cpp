#include <vector>
#include <gl_loader/gl_loader.h>
#include <shaders/shader.h>
#include "shaderimpl.h"
#include <util/files.h>
#include <util/debug.h>

using namespace Shaders;

//-----------------------------------------------------------------------------

static std::vector<char> CommonSource;

typedef std::vector<ShaderParamImpl> ShaderParamList;

//-----------------------------------------------------------------------------

static GLuint CompileShaderStage(GLenum type, const std::vector<char>& source);
static bool LinkShader(const std::vector<GLuint>& stages, GLuint& handle);
static void GetParameters(Shader shader, ShaderParamList& params);

//-----------------------------------------------------------------------------

Shader ShaderClass::Load(const std::string& filename)
{
	if (CommonSource.empty())
	{
		if (!Utils::File::Load("shaders\\common.glsl", CommonSource))
		{
			ASSERTM(false, "failed to load common GLSL source code\n");
			return NULL;
		}
	}

	// load the names of the stages making up the shader
	std::vector<std::string> files;
	Utils::File::ListFiles("shaders\\" + filename, files);
	if (files.empty())
	{
		ASSERTM(false, "no shader stages defined for %s\n", filename);
		return NULL;
	}

	// compile each stage...
	std::vector<GLuint> stages(files.size());
	for (size_t i = 0; i < files.size(); ++i)
	{
		std::vector<char> sourceCode;
		if (!Utils::File::Load(files[i], sourceCode))
		{
			ASSERTM(false, "failed to load shader stage %s\n", files[i]);
			return NULL;
		}

		if (std::string::npos != files[i].find("vs.glsl"))
		{
			stages[i] = CompileShaderStage(GL_VERTEX_SHADER, sourceCode);
		}
		else if (std::string::npos != files[i].find("gs.glsl"))
		{
			stages[i] = CompileShaderStage(GL_GEOMETRY_SHADER, sourceCode);
		}
		else if (std::string::npos != files[i].find("fs.glsl"))
		{
			stages[i] = CompileShaderStage(GL_FRAGMENT_SHADER, sourceCode);
		}
	}

	// link the stages into the final shader...
	GLuint handle;
	if (LinkShader(stages, handle))
	{
		Shader shader(new ShaderClassImpl(handle));
		return shader;
	}

	return NULL;
}

//-----------------------------------------------------------------------------

static GLuint CompileShaderStage(GLenum type, const std::vector<char>& source)
{
	const char* parts[] =
	{
		CommonSource.data(),
		source.data()
	};

	const GLuint stage = glCreateShader(type);
	glShaderSource(stage, 2, parts, NULL);
	glCompileShader(stage);

	GLint status;
	glGetShaderiv(stage, GL_COMPILE_STATUS, &status);
	if (GL_TRUE == status)
	{
		LOG("succeeded\n");
	}
	else
	{
		GLint logLength;
		glGetShaderiv(stage, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<GLchar> log(logLength+1);
		glGetShaderInfoLog(stage, logLength, NULL, log.data());
		LOG("failed:\n%s\n", log.data());
	}

	return stage;
}

//-----------------------------------------------------------------------------

static bool LinkShader(const std::vector<GLuint>& stages, GLuint& handle)
{
	bool success = true;
	
	handle = glCreateProgram();

	for (size_t i = 0; i < stages.size(); ++i) { glAttachShader(handle, stages[i]); }
	
	glLinkProgram(handle);

	GLint status;
	glGetProgramiv(handle, GL_LINK_STATUS, &status);
	if (GL_FALSE == status)
	{
		success = false;
		GLint logLength;
		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<GLchar> log(logLength+1);
		glGetProgramInfoLog(handle, logLength, NULL, log.data());
		LOG("%s\n", log.data());
	}

	for (size_t i = 0; i < stages.size(); ++i)
	{
		glDeleteShader(stages[i]);
		glDetachShader(handle, stages[i]);
	}

	return success;
}

//-----------------------------------------------------------------------------
