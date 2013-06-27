#include <shaders.h>
#include <string>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <debug.h>
#include <glm/gtc/type_ptr.hpp>

//----------------------------------------------------------

struct ShaderParameter
{
	bool dirty;       // whether the cache is out of step with the GPU side, forcing an upload on Shader::Apply if it is
	GLuint location;  // as given by the "layout (location = n)" attribute declaration
	GLenum type;      // the type of the data stored in the attribute (gleaned from the shader program itself)
	uint8_t data[16 * sizeof(double)]; // CPU cache for quick recall
	GLchar name[32];  // the name of the parameter (gleaned from the shader program itself)
};

//----------------------------------------------------------

static GLuint CompileShader(GLenum type, const std::string& filename);
static bool LinkProgram(const std::vector<GLuint>& shaders, GLuint* program);
static bool LoadSourceFile(const std::string& filename, std::string& content);
static void CacheParam(Shader::Parameter param, const void* data, size_t size);

//----------------------------------------------------------

const std::string commonCode(
	"#version 330\n"
	"uniform mat4 ModelViewProjection;\n"
	);

//----------------------------------------------------------

boost::shared_ptr<Shader> Shader::New()
{
	boost::shared_ptr<Shader> shader(new Shader());
	return shader;
}

//----------------------------------------------------------

Shader::Shader()
	: id(-1), params(NULL)
{
}

//----------------------------------------------------------

Shader::~Shader()
{
	glDeleteProgram(id);
}

//----------------------------------------------------------

void Shader::Apply()
{
	glUseProgram(id);
  for (size_t i = 0; i < params.size(); ++i)
  {
    if (params[i].dirty)
    {
      switch (params[i].type)
      {
      case GL_FLOAT:      glUniform1fv(params[i].location, 1, (float*)params[i].data); break;
      case GL_FLOAT_VEC2: glUniform2fv(params[i].location, 1, (float*)params[i].data); break;
      case GL_FLOAT_VEC3: glUniform3fv(params[i].location, 1, (float*)params[i].data); break;
      case GL_FLOAT_VEC4: glUniform4fv(params[i].location, 1, (float*)params[i].data); break;
      case GL_FLOAT_MAT3: glUniformMatrix3fv(params[i].location, 1, GL_FALSE, (float*)params[i].data); break;
      case GL_FLOAT_MAT4: glUniformMatrix4fv(params[i].location, 1, GL_FALSE, (float*)params[i].data); break;
      default: ASSERTM(false, "unsupported type used in shader"); break;
      }
      params[i].dirty = false;
    }
  }
}

//----------------------------------------------------------

bool Shader::Load(const std::string& vsSrc, const std::string& fsSrc)
{
	std::vector<GLuint> shaders;

	shaders.push_back(CompileShader(GL_VERTEX_SHADER, vsSrc));
	shaders.push_back(CompileShader(GL_FRAGMENT_SHADER, fsSrc));

	const bool success = LinkProgram(shaders, &id);

	// Whatever happens, we no longer need the compilation units...
	std::for_each(shaders.begin(), shaders.end(), glDeleteShader);

	if (success)
	{
    GLint numParams;
    glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &numParams);
		params.resize(numParams);
    if (numParams > 0)
    {
			// Get information about the active shader uniform values...
			std::vector<GLuint> indices(numParams);
			std::vector<GLint>  nameLengths(numParams);
			std::vector<GLint>  blockIndices(numParams);
			std::vector<GLint>  types(numParams);
			for (int i = 0; i < numParams; ++i) { indices[i] = i; }
			glGetActiveUniformsiv(id, numParams, indices.data(), GL_UNIFORM_BLOCK_INDEX, blockIndices.data());
			glGetActiveUniformsiv(id, numParams, indices.data(), GL_UNIFORM_NAME_LENGTH, nameLengths.data());
			glGetActiveUniformsiv(id, numParams, indices.data(), GL_UNIFORM_TYPE, types.data());

			for (int i = 0; i < numParams; ++i)
			{
				// Only handling non block-based variables for now...
				if (-1 == blockIndices[i])
				{
					glGetActiveUniformName(id, i, sizeof(params[0].name) - 1, NULL, params[i].name);
					params[i].location = glGetUniformLocation(id, params[i].name);
					params[i].type = types[i];
				}
			}
		}
	}

	return success;
}

//----------------------------------------------------------

Shader::Parameter Shader::GetParameter(const char* name) const
{
	for (size_t i = 0; (i < params.size()); ++i)
	{
		if (0 == strcmp(params[i].name, name))
		{
			return (Shader::Parameter)&params[i];
		}
	}
	return NULL;
}

//----------------------------------------------------------

void Shader::SetParam(Parameter param, float value) { CacheParam(param, &value, sizeof(value)); }
void Shader::SetParam(Parameter param, const glm::vec2& value) { CacheParam(param, glm::value_ptr(value), sizeof(value)); }
void Shader::SetParam(Parameter param, const glm::vec3& value) { CacheParam(param, glm::value_ptr(value), sizeof(value)); }
void Shader::SetParam(Parameter param, const glm::vec4& value) { CacheParam(param, glm::value_ptr(value), sizeof(value)); }
void Shader::SetParam(Parameter param, const glm::mat2& value) { CacheParam(param, glm::value_ptr(value), sizeof(value)); }
void Shader::SetParam(Parameter param, const glm::mat3& value) { CacheParam(param, glm::value_ptr(value), sizeof(value)); }
void Shader::SetParam(Parameter param, const glm::mat4& value) { CacheParam(param, glm::value_ptr(value), sizeof(value)); }

void Shader::SetParam(const char* name, float value) { CacheParam(GetParameter(name), &value, sizeof(value)); }
void Shader::SetParam(const char* name, const glm::vec2& value) { CacheParam(GetParameter(name), glm::value_ptr(value), sizeof(value)); }
void Shader::SetParam(const char* name, const glm::vec3& value) { CacheParam(GetParameter(name), glm::value_ptr(value), sizeof(value)); }
void Shader::SetParam(const char* name, const glm::vec4& value) { CacheParam(GetParameter(name), glm::value_ptr(value), sizeof(value)); }
void Shader::SetParam(const char* name, const glm::mat2& value) { CacheParam(GetParameter(name), glm::value_ptr(value), sizeof(value)); }
void Shader::SetParam(const char* name, const glm::mat3& value) { CacheParam(GetParameter(name), glm::value_ptr(value), sizeof(value)); }
void Shader::SetParam(const char* name, const glm::mat4& value) { CacheParam(GetParameter(name), glm::value_ptr(value), sizeof(value)); }

//----------------------------------------------------------

static void CacheParam(Shader::Parameter param, const void* data, size_t size)
{
	if (NULL != param)
	{
		if (0 != memcmp(param->data, data, size))
		{
			memcpy(param->data, data, size);
			param->dirty = true;
		}
	}
	else
	{
		ASSERTM(false, "invalid shader parameter");
	}
}

//----------------------------------------------------------

static GLuint CompileShader(GLenum type, const std::string& filename)
{
	GLuint shader = glCreateShader(type);

	// Load the shader source code from file...
	const std::string fullPath("shaders\\" + filename);
	std::string src;
	if (LoadSourceFile(fullPath, src))
	{
		src.insert(0, commonCode);
		const char* srcCode = src.data();
		// Create the shader program, load the file and compile it, then report any errors...
		glShaderSource(shader, 1, &srcCode, NULL);

		LOG("compilation of %s", fullPath.c_str());
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
