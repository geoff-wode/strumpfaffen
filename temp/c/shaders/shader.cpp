#include <gl_loader/gl_loader.h>
#include <dirent.h>
#include <vector>
#include <core/debug.h>
#include <shaders/shader.h>
#include <algorithm>

using namespace Shaders;

// -------------------------------------------------------------------------

static bool LoadShaderSource(const std::string& path, std::string& src);
static bool GetShaderStageNames(const std::string& path, std::vector<std::string>& stages);
static GLuint CompileShaderStage(GLenum type, const std::string& src);
static bool LinkShader(const std::vector<GLuint>& stages, GLuint& handle);
static void GetShaderVertexAttributes(GLuint handle, ShaderVertexAttributeList& attrs);
static void GetShaderParameters(GLuint handle, std::vector<class ShaderParameterImpl>& params);

//-----------------------------------------------------------------------------

class ShaderParameterImpl : public IShaderParameter
{
public:
	ShaderParameterImpl()
		: dirty(false)
	{
	}

	const std::string& ShaderParameterImpl::GetName() const { return name; }

	void ShaderParameterImpl::Set(float value) { Cache(&value, sizeof(value)); }
	void ShaderParameterImpl::Set(const glm::vec2& value) { Cache(&value, sizeof(value)); }
	void ShaderParameterImpl::Set(const glm::vec3& value) { Cache(&value, sizeof(value)); }
	void ShaderParameterImpl::Set(const glm::vec4& value) { Cache(&value, sizeof(value)); }
	void ShaderParameterImpl::Set(const glm::mat2& value) { Cache(&value, sizeof(value)); }
	void ShaderParameterImpl::Set(const glm::mat3& value) { Cache(&value, sizeof(value)); }
	void ShaderParameterImpl::Set(const glm::mat4& value) { Cache(&value, sizeof(value)); }

	float ShaderParameterImpl::GetFloat() { return *(float*)cache; }
	glm::vec2 ShaderParameterImpl::GetVec2() { return *(glm::vec2*)cache; }
	glm::vec3 ShaderParameterImpl::GetVec3() { return *(glm::vec3*)cache; }
	glm::vec4 ShaderParameterImpl::GetVec4() { return *(glm::vec4*)cache; }
	glm::mat2 ShaderParameterImpl::GetMat2() { return *(glm::mat2*)cache; }
	glm::mat3 ShaderParameterImpl::GetMat3() { return *(glm::mat3*)cache; }
	glm::mat4 ShaderParameterImpl::GetMat4() { return *(glm::mat4*)cache; }

	void ShaderParameterImpl::Update()
	{
		if (dirty)
		{
			dirty = false;
			switch (type)
			{
			case GL_FLOAT:		  glUniform1fv(location, 1, (float*)cache); break;
			case GL_FLOAT_VEC2: glUniform2fv(location, 1, (float*)cache); break;
			case GL_FLOAT_VEC3: glUniform3fv(location, 1, (float*)cache); break;
			case GL_FLOAT_VEC4: glUniform4fv(location, 1, (float*)cache); break;
			case GL_FLOAT_MAT2: glUniformMatrix2fv(location, 1, GL_FALSE, (float*)cache); break;
			case GL_FLOAT_MAT3: glUniformMatrix3fv(location, 1, GL_FALSE, (float*)cache); break;
			case GL_FLOAT_MAT4: glUniformMatrix4fv(location, 1, GL_FALSE, (float*)cache); break;
			default: LOG("unsupported shader param type\n"); break;
			}
		}
	}

	void ShaderParameterImpl::Cache(const void* const value, size_t size)
	{
		if (0 != memcmp(value, cache, size))
		{
			memcpy(cache, value, size);
			dirty = true;
		}
	}

	bool dirty;
	std::string name;
	GLuint location;
	GLenum type;
	double cache[16];
};

//------------------------------------------------------------------------------------------

class ShaderImpl : public IShader
{
public:
	ShaderImpl(GLuint handle)
		: handle(handle)
	{
		GetShaderVertexAttributes(handle, vertexAttributes);
		GetShaderParameters(handle, parameters);
	}

	virtual ~ShaderImpl()
	{
		glDeleteProgram(handle);
	}

	virtual const ShaderVertexAttribute* const GetVertexAttribute(const std::string& name) const
	{
		for (size_t i = 0; i < vertexAttributes.size(); ++i)
		{
			if (name == vertexAttributes[i].Name) { return &vertexAttributes[i]; }
		}
		return NULL;
	}

	virtual IShaderParameter* const GetParameter(const std::string& name) const
	{
		for (size_t i = 0; i < parameters.size(); ++i)
		{
			if (name == parameters[i].name) { return (IShaderParameter* const)&parameters[i]; }
		}
		return NULL;
	}

	virtual void Activate()
	{
		glUseProgram(handle);

		for (size_t i = 0; i < parameters.size(); ++i)
		{
			parameters[i].Update();
		}
	}

	GLuint handle;
	std::vector<ShaderParameterImpl> parameters;
	ShaderVertexAttributeList vertexAttributes;

	static std::string commonSrc;
};

std::string ShaderImpl::commonSrc;

// -------------------------------------------------------------------------

Shader Shaders::LoadShader(const std::string& name)
{
	if (ShaderImpl::commonSrc.empty())
	{
		LoadShaderSource("shaders\\common.glsl", ShaderImpl::commonSrc);
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
			shaderStages[i] = CompileShaderStage(GL_VERTEX_SHADER, src);
		}
		else if (std::string::npos != shaderStageNames[i].find("gs.glsl"))
		{
			shaderStages[i] = CompileShaderStage(GL_GEOMETRY_SHADER, src);
		}
		else if (std::string::npos != shaderStageNames[i].find("fs.glsl"))
		{
			shaderStages[i] = CompileShaderStage(GL_FRAGMENT_SHADER, src);
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
	const char* srcCode[] =
	{
		ShaderImpl::commonSrc.c_str(),
		src.c_str()
	};

	GLuint shader = glCreateShader(type);

	glShaderSource(shader, 2, srcCode, NULL);
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

static void GetShaderVertexAttributes(GLuint program, ShaderVertexAttributeList& attrs)
{
	GLint numAttributes;
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numAttributes);
	attrs.resize(numAttributes);

	if (numAttributes > 0)
	{
		GLint maxNameLength;
		glGetProgramiv(program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxNameLength);
		std::vector<char> attrName(maxNameLength);

		unsigned int index = 0;
		for (GLint i = 0; i < numAttributes; ++i)
		{
			GLint numComponents;
			GLenum type;

			glGetActiveAttrib(program, i, maxNameLength, NULL, &numComponents, &type, attrName.data());

			// Ignore built-in GLSL attributes (starting with "gl_")...
			if (0 != strcmp("gl_", attrName.data()))
			{
				attrs[index].Name = attrName.data();
				attrs[index].Location = glGetAttribLocation(program, attrName.data());
				attrs[index].Type = type;
				++index;
			}
		}
	}
}

//------------------------------------------------------------------------------------------

static void GetShaderParameters(GLuint program, std::vector<ShaderParameterImpl>& params)
{
	GLint numParams;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numParams);
	params.resize(numParams);

	if (numParams > 0)
	{
		GLint maxNameLength;
		glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);
		std::vector<char>	paramName(maxNameLength);
	
		std::vector<GLuint>	indices(numParams);
		std::vector<GLint>	blockIndices(numParams);
		std::vector<GLint>	types(numParams);

		for (GLint i = 0; i < numParams; ++i ) { indices[i] = i; }

		glGetActiveUniformsiv(program, numParams, indices.data(), GL_UNIFORM_BLOCK_INDEX, blockIndices.data());
		glGetActiveUniformsiv(program, numParams, indices.data(), GL_UNIFORM_TYPE, types.data());

		unsigned int index = 0;
		for (GLint i = 0; i < numParams; ++i)
		{
			if (-1 == blockIndices[i])
			{
				glGetActiveUniformName(program, i, maxNameLength, NULL, paramName.data());

				params[i].name = paramName.data();
				params[i].location = glGetUniformLocation(program, paramName.data());
				params[i].type = types[i];
				++index;
			}
		}
	}
}
