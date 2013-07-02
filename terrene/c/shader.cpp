#include <shader/shader.h>
#include <dirent.h>
#include <list>
#include <gl_loader/gl_loader.h>
#include <debug.h>
#include <vector>
#include <map>
#include <algorithm>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <memory>

using namespace Terrene;

//--------------------------------------------------------------------------------------

typedef std::vector<ShaderParameter> ShaderParameterList;
typedef std::vector<ShaderAttribute> ShaderAttributeList;

// Shaders are composed of between 1 and 3 files, each corresponding to one of the pipeline stages
// (vertex, geometry or fragment). Each stage's source code is contained in a file called fs.glsl, gs.glsl
// or vs.glsl respectively. All 3 files exist in a containing directory which "defines" the overall shader
// program. That directory's name is also the name of the shader as known to the rest of the program.
// This function queries that directory and returns the names of any files it contains.
static void GetShaderFileNames(const std::string& shaderName, std::list<std::string>& filenames);

static void LoadSourceFile(const std::string& path, std::string& content);
static GLuint CompileShader(GLenum type, const std::string& path);
static bool LinkProgram(const std::vector<GLuint>& shaders, GLuint* handle);
static void GetUniformParameters(GLuint shader, ShaderParameterList& parameters);
static void GetShaderAttributes(GLuint shader, ShaderAttributeList& attributes);

//--------------------------------------------------------------------------------------

class ShaderImpl : public IShader
{
public:
	ShaderImpl(GLuint handle)
		: handle(handle)
	{
		GetUniformParameters(handle, parameters);
		GetShaderAttributes(handle, attributes);
	}

	virtual ~ShaderImpl()
	{
		glDeleteProgram(handle);
	}

	virtual ShaderParameter GetParameter(const std::string& name) const
	{
		for (ShaderParameterList::const_iterator i = parameters.begin(); i != parameters.end(); ++i)
		{
			if (name == (*i)->GetName())
			{
				return *i;
			}
		}
		return NULL;
	}

	virtual ShaderAttribute GetAttribute(const std::string& name) const
	{
		for (size_t i = 0; i < attributes.size(); ++i)
		{
			if (name == attributes.at(i)->Name)
			{
				return attributes[i];
			}
		}
		return NULL;
	}

	virtual void Activate()
	{
		glUseProgram(handle);
		for (ShaderParameterList::iterator i = parameters.begin(); i != parameters.end(); ++i)
		{
			(*i)->Update();
		}
	}

	static bool commonCodeLoaded;
	static std::string commonCode;

private:
	GLuint handle;
	ShaderParameterList parameters;
	ShaderAttributeList attributes;
};

bool ShaderImpl::commonCodeLoaded = false;
std::string ShaderImpl::commonCode;

//--------------------------------------------------------------------------------------

class ShaderParameterImpl : public IShaderParameter
{
public:
	const std::string& GetName() const { return name; }

	ShaderParameterImpl(const std::string& name, GLenum type, GLuint location)
		: name(name), type(type), location(location), dirty(false)
	{
		(void)memset(cache, 0, sizeof(cache));
	}

	virtual ~ShaderParameterImpl()
	{
	}

	virtual void Set(float value) { CacheValue(&value, sizeof(value)); }
	virtual void Set(const glm::vec2& value) { CacheValue(&value, sizeof(value)); }
	virtual void Set(const glm::vec3& value) { CacheValue(&value, sizeof(value)); }
	virtual void Set(const glm::vec4& value) { CacheValue(&value, sizeof(value)); }
	virtual void Set(const glm::mat2& value) { CacheValue(&value, sizeof(value)); }
	virtual void Set(const glm::mat3& value) { CacheValue(&value, sizeof(value)); }
	virtual void Set(const glm::mat4& value) { CacheValue(&value, sizeof(value)); }

	virtual float GetFloat() { float value; GetValue(&value, sizeof(value)); return value; }
	virtual glm::vec2 GetVec2() { glm::vec2 value; GetValue(&value, sizeof(value)); return value; }
	virtual glm::vec3 GetVec3() { glm::vec3 value; GetValue(&value, sizeof(value)); return value; }
	virtual glm::vec4 GetVec4() { glm::vec4 value; GetValue(&value, sizeof(value)); return value; }
	virtual glm::mat2 GetMat2() { glm::mat2 value; GetValue(&value, sizeof(value)); return value; }
	virtual glm::mat3 GetMat3() { glm::mat3 value; GetValue(&value, sizeof(value)); return value; }
	virtual glm::mat4 GetMat4() { glm::mat4 value; GetValue(&value, sizeof(value)); return value; }

	virtual void Update()
	{
		if (dirty)
		{
			dirty = false;
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
		}
	}

private:
	void CacheValue(const void* value, size_t sizeInBytes)
	{
		if (0 != memcmp(value, cache, sizeInBytes))
		{
			dirty = true;
			memcpy(cache, value, sizeInBytes);
		}
	}

	void GetValue(void* value, size_t sizeInBytes) { memcpy(value, cache, sizeInBytes); }

	std::string name;
	const GLenum type;
	const GLuint location;
	bool dirty;
	unsigned char cache[sizeof(glm::mat4)];
};

//--------------------------------------------------------------------------------------

static void GetShaderAttributes(GLuint shader, ShaderAttributeList& attributes)
{
	attributes.clear();

	GLint numAttributes;
	glGetProgramiv(shader, GL_ACTIVE_ATTRIBUTES, &numAttributes);

	GLint maxNameLength = 0;
	glGetProgramiv(shader, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxNameLength);
	std::vector<GLchar> attributeName(maxNameLength+1);

	for (GLint i = 0; i < numAttributes; ++i)
	{
		GLint numComponents;
		GLenum type;
		glGetActiveAttrib(shader, i, maxNameLength, NULL, &numComponents, &type, attributeName.data());
		if (0 != strcmp("gl_", attributeName.data())) // ignore built-in attributes...
		{
			GLint location = glGetAttribLocation(shader, attributeName.data());

			ShaderAttribute attr(new SShaderAttribute(attributeName.data(), type, location, numComponents));
			attributes.push_back(attr);
		}
	}
}

//--------------------------------------------------------------------------------------

static void GetUniformParameters(GLuint shader, ShaderParameterList& parameters)
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
				
				ShaderParameter p(new ShaderParameterImpl(name, type, location));
				parameters[index] = p;
				++index;
			}
		}
	}
}

//--------------------------------------------------------------------------------------

Shader Terrene::LoadShader(const std::string& name)
{
	// get all the files in the shader directory called 'name'...
	std::list<std::string> filenames;
	GetShaderFileNames(name, filenames);

	// compile each file according to its type...
	std::vector<GLuint> shaders;
	for (std::list<std::string>::const_iterator i = filenames.begin(); i != filenames.end(); ++i)
	{
		if (std::string::npos != i->find("vs."))
		{
			shaders.push_back(CompileShader(GL_VERTEX_SHADER, *i));
		}
		else if (std::string::npos != i->find("fs."))
		{
			shaders.push_back(CompileShader(GL_FRAGMENT_SHADER, *i));
		}
		else if (std::string::npos != i->find("gs."))
		{
			shaders.push_back(CompileShader(GL_GEOMETRY_SHADER, *i));
		}
	}

	GLuint handle;
	const bool linkedOK = LinkProgram(shaders, &handle);

	std::for_each(shaders.begin(), shaders.end(), glDeleteShader);

	if (linkedOK)
	{
		Shader s(new ShaderImpl(handle));
		return s;
	}

	return NULL;
}

//--------------------------------------------------------------------------------------

static GLuint CompileShader(GLenum type, const std::string& path)
{

	if (!ShaderImpl::commonCodeLoaded)
	{
		LoadSourceFile("shaders\\common.glsl", ShaderImpl::commonCode);
		ShaderImpl::commonCodeLoaded = true;
	}

	GLuint shader = glCreateShader(type);

	// Load the shader source code from file and pre-pend the common code...
	std::string src;
	LoadSourceFile(path, src);
	src.insert(0, ShaderImpl::commonCode);

	const char* srcCode = src.data();
	// Create the shader program, load the file and compile it, then report any errors...
	glShaderSource(shader, 1, &srcCode, NULL);

	LOG("compilation of %s ", path.c_str());
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

//--------------------------------------------------------------------------------------

static bool LinkProgram(const std::vector<GLuint>& shaders, GLuint* handle)
{
	bool success = true;

	*handle = glCreateProgram();
	for (size_t i = 0; i < shaders.size(); ++i) { glAttachShader(*handle, shaders[i]); }
	glLinkProgram(*handle);

	GLint status;
	glGetProgramiv(*handle, GL_LINK_STATUS, &status);
	if (GL_FALSE == status)
	{
		success = false;
		GLint logLength;
		glGetProgramiv(*handle, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<GLchar> log(logLength+1);
		glGetProgramInfoLog(*handle, logLength, NULL, log.data());
		LOG("%s\n", log.data());
	}

	for (size_t i = 0; i < shaders.size(); ++i) { glDetachShader(*handle, shaders[i]); }
	return success;
}

//--------------------------------------------------------------------------------------

static void LoadSourceFile(const std::string& path, std::string& content)
{
	FILE* in = fopen(path.c_str(), "rb");
	if (NULL != in)
	{
		fseek(in, 0, SEEK_END);
		content.resize(ftell(in));
		rewind(in);
		fread(&content[0], sizeof(char), content.size(), in);
		fclose(in);
	}
}

//--------------------------------------------------------------------------------------

static void GetShaderFileNames(const std::string& shaderName, std::list<std::string>& filenames)
{
	filenames.clear();

	std::string pathToShader("shaders\\" + shaderName);

	DIR* dir = opendir(pathToShader.c_str());
	if (dir)
	{
		for (struct dirent* ent = readdir(dir); NULL != ent; ent = readdir(dir))
		{
			if ((0 != strcmp(ent->d_name, ".")) && (0 != strcmp(ent->d_name, "..")))
			{
				std::string filename(ent->d_name);
				filenames.push_back(pathToShader + "\\" + filename);
			}
		}
		closedir(dir);
	}
}
