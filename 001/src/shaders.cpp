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

const std::string commonCode(
	"#version 330\n"
	"uniform mat4 ModelViewProjection;\n"
	);

//----------------------------------------------------------

bool NewShaderProgram(const std::string& vsSrc, const std::string& fsSrc, Shader* shader)
{
	Shader newShader(new ShaderData());

	std::vector<GLuint> shaders;

	shaders.push_back(CompileShader(GL_VERTEX_SHADER, vsSrc));
	shaders.push_back(CompileShader(GL_FRAGMENT_SHADER, fsSrc));

	const bool success = LinkProgram(shaders, &newShader->id);

	// Whatever happens, we no longer need the compilation units...
	std::for_each(shaders.begin(), shaders.end(), glDeleteShader);

	if (success)
	{
		*shader = newShader;

    GLint numParams;
    glGetProgramiv(newShader->id, GL_ACTIVE_UNIFORMS, &numParams);
    if (numParams > 0)
    {
      newShader->params.resize(numParams);
			// Get information about the active shader uniform values...
			std::vector<GLuint> indices(numParams);
			std::vector<GLint>  nameLengths(numParams);
			std::vector<GLint>  blockIndices(numParams);
			std::vector<GLint>  types(numParams);
			for (int i = 0; i < numParams; ++i) { indices[i] = i; }
			glGetActiveUniformsiv(newShader->id, numParams, indices.data(), GL_UNIFORM_BLOCK_INDEX, blockIndices.data());
			glGetActiveUniformsiv(newShader->id, numParams, indices.data(), GL_UNIFORM_NAME_LENGTH, nameLengths.data());
			glGetActiveUniformsiv(newShader->id, numParams, indices.data(), GL_UNIFORM_TYPE, types.data());

			for (int i = 0; i < numParams; ++i)
			{
				// Only handling non block-based variables for now...
				if (-1 == blockIndices[i])
				{
					glGetActiveUniformName(newShader->id, i, sizeof(newShader->params[0].name) - 1, NULL, newShader->params[i].name);
					newShader->params[i].location = glGetUniformLocation(newShader->id, newShader->params[i].name);
					newShader->params[i].type = types[i];
				}
			}
		}
	}

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
		src.insert(0, commonCode);
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
