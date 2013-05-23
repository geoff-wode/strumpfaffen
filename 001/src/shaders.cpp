#include <shaders.h>
#include <string>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <debug.h>

//----------------------------------------------------------

static GLuint CompileShader(GLenum type, const std::string& filename);
static bool LinkProgram(const std::vector<GLuint>& shaders, GLuint* program);
static bool LoadSourceFile(const std::string& filename, std::string& content);

//----------------------------------------------------------

bool NewShaderProgram(const std::string& vsSrc, const std::string& fsSrc, GLuint* program)
{
	std::vector<GLuint> shaders;

	shaders.push_back(CompileShader(GL_VERTEX_SHADER, vsSrc));
	shaders.push_back(CompileShader(GL_FRAGMENT_SHADER, fsSrc));

	const bool success = LinkProgram(shaders, program);

	// Whatever happens, we no longer need the compilation units...
	std::for_each(shaders.begin(), shaders.end(), glDeleteShader);

	return success;
}

//----------------------------------------------------------

static GLuint CompileShader(GLenum type, const std::string& filename)
{
	GLuint shader = glCreateShader(type);

	// Load the shader source code from file...
	std::string src;
	if (LoadSourceFile("shaders\\" + filename, src))
	{
		const char* srcCode = src.data();
		// Create the shader program, load the file and compile it, then report any errors...
		glShaderSource(shader, 1, &srcCode, NULL);

		LOG("compilation of %s", filename.c_str());
		glCompileShader(shader);

		GLint status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (GL_TRUE == status)
		{
			LOG(" succeeded\n");
		}
		else
		{
			GLint logLength;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
			std::vector<GLchar> log(logLength+1);
			glGetShaderInfoLog(shader, logLength, NULL, log.data());
			LOG(" failed:\n%s\n", log.data());
		}
	}
	return shader;
}

static bool LinkProgram(const std::vector<GLuint>& shaders, GLuint* program)
{
	bool success = true;

	*program = glCreateProgram();
	for (size_t i = 0; i < shaders.size(); ++i) { glAttachShader(*program, shaders[i]); }
	glLinkProgram(*program);

	GLint status;
	glGetProgramiv(*program, GL_LINK_STATUS, &status);
	if (GL_FALSE == status)
	{
		success = false;
		GLint logLength;
		glGetProgramiv(*program, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<GLchar> log(logLength+1);
		glGetProgramInfoLog(*program, logLength, NULL, log.data());
		LOG("%s\n", log.data());
	}

	for (size_t i = 0; i < shaders.size(); ++i) { glDetachShader(*program, shaders[i]); }
	return success;
}

static bool LoadSourceFile(const std::string& filename, std::string& content)
{
	FILE* in = fopen(filename.c_str(), "rb");
	if (NULL != in)
	{
		fseek(in, 0, SEEK_END);
		content.resize(ftell(in));
		rewind(in);
		fread(&content[0], sizeof(char), content.size(), in);
		fclose(in);
	}
	return (NULL != in);
}
