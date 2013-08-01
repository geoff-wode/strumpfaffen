#include <shaderX/shader.h>
#include <dirent.h>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <debug.h>
#include <boost/scoped_ptr.hpp>
#include <memory>

using namespace Terrene::Shaders;

//------------------------------------------------------------------------------------------

static std::string commonSrc;

//------------------------------------------------------------------------------------------

static bool LoadShaderSource(const std::string& path, std::string& src);
static bool GetShaderStageNames(const std::string& path, std::vector<std::string>& stages);
static GLuint CompileShaderStage(GLenum type, const std::string& src);
static bool LinkShader(const std::vector<GLuint>& stages, GLuint& handle);
static void GetVertexAttributes(GLuint shader, AttributeList& attributes);
static void GetShaderParameters(GLuint shader, std::vector<class ParameterImpl>& parameters);

//------------------------------------------------------------------------------------------

class ParameterImpl : public IParameter
{
public:
	ParameterImpl() { }

	ParameterImpl(const std::string& name, GLuint location, GLenum type)
		: dirty(false), name(name), location(location), type(type)
	{
	}

	virtual const std::string& GetName() const { return name; }

	virtual void Set(float value) { CacheValue(&value, sizeof(value)); }
	virtual void Set(const glm::vec2& value) { CacheValue(&value, sizeof(value)); }
	virtual void Set(const glm::vec3& value) { CacheValue(&value, sizeof(value)); }
	virtual void Set(const glm::vec4& value) { CacheValue(&value, sizeof(value)); }
	virtual void Set(const glm::mat2& value) { CacheValue(&value, sizeof(value)); }
	virtual void Set(const glm::mat3& value) { CacheValue(&value, sizeof(value)); }
	virtual void Set(const glm::mat4& value) { CacheValue(&value, sizeof(value)); }

	virtual float GetFloat() { return *(float*)cache; }
	virtual glm::vec2 GetVec2() { return *(glm::vec2*)cache; }
	virtual glm::vec3 GetVec3() { return *(glm::vec3*)cache; }
	virtual glm::vec4 GetVec4() { return *(glm::vec4*)cache; }
	virtual glm::mat2 GetMat2() { return *(glm::mat2*)cache; }
	virtual glm::mat3 GetMat3() { return *(glm::mat3*)cache; }
	virtual glm::mat4 GetMat4() { return *(glm::mat4*)cache; }

	virtual void Update()
	{
		if (dirty)
		{
			switch (type)
			{
      case GL_FLOAT:      glUniform1fv(location, 1, (float*)cache); break;
      case GL_FLOAT_VEC2: glUniform2fv(location, 1, (float*)cache); break;
      case GL_FLOAT_VEC3: glUniform3fv(location, 1, (float*)cache); break;
      case GL_FLOAT_VEC4: glUniform4fv(location, 1, (float*)cache); break;
      case GL_FLOAT_MAT3: glUniformMatrix3fv(location, 1, GL_FALSE, (float*)cache); break;
      case GL_FLOAT_MAT4: glUniformMatrix4fv(location, 1, GL_FALSE, (float*)cache); break;
      default: ASSERTM(false, "unsupported type used in shader"); break;
			}
			dirty = false;
		}
	}

private:
	void CacheValue(const void* const value, size_t size)
	{
		if (0 != memcmp(value, cache, size))
		{
			dirty = true;
			memcpy(cache, value, size);
		}
	}

	bool				dirty;
	std::string name;
	GLuint			location;
	GLenum			type;
	GLubyte			cache[sizeof(double) * 16];
};

//------------------------------------------------------------------------------------------

class ShaderImpl : public IShader
{
public:
	ShaderImpl(GLuint handle)
		: handle(handle)
	{
		GetVertexAttributes(handle, vertexAttributes);
		GetShaderParameters(handle, parameters);
	}

	virtual ~ShaderImpl()
	{
		glDeleteProgram(handle);
	}

	virtual const AttributeList& VertexAttributes() const
	{
		return vertexAttributes;
	}

	virtual const unsigned int FragmentOutput(const std::string& name) const
	{
		return glGetFragDataLocation(handle, name.c_str());
	}

	virtual Parameter GetParameter(const std::string& name) const
	{
		for (size_t i = 0; (i < parameters.size()); ++i)
		{
			if (name == parameters[i].GetName()) { return (Parameter) &parameters[i]; }
		}
		return NULL;
	}

	virtual void Activate()
	{
		glUseProgram(handle);
		for (size_t i = 0; i < parameters.size(); ++i) { parameters[i].Update(); }
	}

	GLuint handle;
	AttributeList	vertexAttributes;
	unsigned int	numParams;
	std::vector<ParameterImpl> parameters;
};

//------------------------------------------------------------------------------------------

Shader Terrene::Shaders::LoadShader(const std::string& name)
{
	if (commonSrc.empty())
	{
		LoadShaderSource("shaders\\common.glsl", commonSrc);
	}

	// Get the filenames of each of the stages in the shader (each stage is in a seperate file)...
	std::vector<std::string> shaderStageNames;
	GetShaderStageNames("shaders\\" + name, shaderStageNames);

	// Compile each stage and add it to a list...
	std::vector<GLuint> shaderStages(shaderStageNames.size());
	for (size_t i = 0; i < shaderStageNames.size(); ++i)
	{
		LOG("compilation of %s ", shaderStageNames[i].c_str());

		std::string src;
		LoadShaderSource(shaderStageNames[i], src);

		if (std::string::npos != shaderStageNames[i].find("vs.glsl"))
		{
			shaderStages[i] = CompileShaderStage(GL_VERTEX_SHADER, commonSrc + src);
		}
		else if (std::string::npos != shaderStageNames[i].find("gs.glsl"))
		{
			shaderStages[i] = CompileShaderStage(GL_GEOMETRY_SHADER, commonSrc + src);
		}
		else if (std::string::npos != shaderStageNames[i].find("fs.glsl"))
		{
			shaderStages[i] = CompileShaderStage(GL_FRAGMENT_SHADER, commonSrc + src);
		}
	}

	// Link the stages into a final shader program and create the class instance if successful...
	GLuint handle;
	if (LinkShader(shaderStages, handle))
	{
		Shader shader(new ShaderImpl(handle));
		return shader;
	}

	return NULL;
}

//------------------------------------------------------------------------------------------

static bool LoadShaderSource(const std::string& path, std::string& src)
{
	FILE* in = fopen(path.c_str(), "rb");
	if (NULL != in)
	{
		fseek(in, 0, SEEK_END);
		src.resize(ftell(in));
		rewind(in);
		fread(&src[0], sizeof(char), src.size(), in);
		fclose(in);
	}

	return !src.empty();
}

//------------------------------------------------------------------------------------------

static bool GetShaderStageNames(const std::string& path, std::vector<std::string>& stages)
{
	const std::string pathToShader(path + "\\");

	DIR* dir = opendir(pathToShader.c_str());
	if (dir)
	{
		for (struct dirent* ent = readdir(dir); NULL != ent; ent = readdir(dir))
		{
			if ((0 != strcmp(ent->d_name, ".")) && (0 != strcmp(ent->d_name, "..")))
			{
				std::string stage(ent->d_name);
				stages.push_back(pathToShader + stage);
			}
		}
		closedir(dir);
	}

	return !stages.empty();
}

//------------------------------------------------------------------------------------------

static GLuint CompileShaderStage(GLenum type, const std::string& src)
{
	GLuint shader = glCreateShader(type);

	const char* srcCode = src.data();
	glShaderSource(shader, 1, &srcCode, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (GL_TRUE == status)
	{
		LOG("succeeded\n");
	}
	else
	{
		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<GLchar> log(logLength+1);
		glGetShaderInfoLog(shader, logLength, NULL, log.data());
		LOG("failed:\n%s\n", log.data());
	}

	return shader;
}

//------------------------------------------------------------------------------------------

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

	for (size_t i = 0; i < stages.size(); ++i) { glDetachShader(handle, stages[i]); }

	return success;
}

//------------------------------------------------------------------------------------------

static void GetVertexAttributes(GLuint shader, AttributeList& attributes)
{
	GLint attrCount;
	glGetProgramiv(shader, GL_ACTIVE_ATTRIBUTES, &attrCount);

	GLint maxAttrNameLen;
	glGetProgramiv(shader, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttrNameLen);
	std::vector<char> attrName(maxAttrNameLen+1);

	for (GLint i = 0; i < attrCount; ++i)
	{
		GLint componentCount;
		GLenum type;

		glGetActiveAttrib(shader, i, maxAttrNameLen, NULL, &componentCount, &type, attrName.data());

		// Built-in reserved GLSL variables are prefixed with 'gl_' and must be ignored (location == -1)...
		if (0 != strcmp("gl_", attrName.data()))
		{
			const GLint location = glGetAttribLocation(shader, attrName.data());

			Terrene::Shaders::VertexAttribute va(attrName.data(), location, type);
			attributes.insert(std::pair<std::string,VertexAttribute>(va.Name, va));
		}
	}
}

//------------------------------------------------------------------------------------------

static void GetShaderParameters(GLuint shader, std::vector<ParameterImpl>& parameters)
{
  GLint numParams;
  glGetProgramiv(shader, GL_ACTIVE_UNIFORMS, &numParams);
	parameters.resize(numParams);

  if (numParams > 0)
  {
		// Get information about the active shader uniform values...
		std::vector<GLuint> indices(numParams);
		std::vector<GLint>  nameLengths(numParams);
		std::vector<GLint>  blockIndices(numParams);
		std::vector<GLint>  types(numParams);

		for (int i = 0; i < numParams; ++i) { indices[i] = i; }
		
		glGetActiveUniformsiv(shader, numParams, indices.data(), GL_UNIFORM_BLOCK_INDEX, blockIndices.data());
		glGetActiveUniformsiv(shader, numParams, indices.data(), GL_UNIFORM_NAME_LENGTH, nameLengths.data());
		glGetActiveUniformsiv(shader, numParams, indices.data(), GL_UNIFORM_TYPE, types.data());

		unsigned int index = 0;
		for (int i = 0; i < numParams; ++i)
		{
			// Only handling non block-based variables for now...
			if (-1 == blockIndices[i])
			{
				GLchar name[128];
				glGetActiveUniformName(shader, i, sizeof(name) - 1, NULL, name);
				const GLuint location = glGetUniformLocation(shader, name);
				const GLenum type = types[i];
				
				ParameterImpl p(name, location, type);
				parameters[index] = p;
				++index;
			}
		}
	}
}
