#include <memory>
#include <vector>
#include <string>
#include <map>
#include <stdio.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <gl_loader/gl_loader.h>
#include "files.h"
#include "debug.h"

//--------------------------------------------------------------------------------------------

static void CompileShader(const std::string& name, std::vector<GLuint>& stages)
{
	static std::vector<char> commonSource;
	if (commonSource.empty()) { File::Load("shaders/common.glsl", commonSource); }

	// load the names of the stages making up the shader
	std::vector<std::string> files;
	File::ListFiles(name, files);
	if (!files.empty())
	{
		const char* parts[] = { commonSource.data(), NULL };
		GLint lengths[] = { commonSource.size(), 0 };

		// compile each stage...
		for (size_t i = 0; i < files.size(); ++i)
		{
			std::vector<char> sourceCode;
			if (File::Load(files[i], sourceCode))
			{
				parts[1] = sourceCode.data();
				lengths[1] = sourceCode.size();

				GLuint shader;
				if (std::string::npos != files[i].find("vs.glsl")) { shader = glCreateShader(GL_VERTEX_SHADER); }
				else if (std::string::npos != files[i].find("gs.glsl")) { shader = glCreateShader(GL_GEOMETRY_SHADER); }
				else if (std::string::npos != files[i].find("fs.glsl")) { shader = glCreateShader(GL_FRAGMENT_SHADER); }
				else { continue; }

				glShaderSource(shader, 2, parts, lengths);
				glCompileShader(shader);

				GLint status;
				glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
				if (GL_TRUE == status)
				{
					LOG("%s compiled ok\n", files[i].c_str());
					stages.push_back(shader);
				}
				else
				{
					GLint logLength;
					glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
					std::vector<GLchar> log(logLength+1);
					glGetShaderInfoLog(shader, logLength, NULL, log.data());
					LOG("%s failed to compile:\n%s\n", files[i].c_str(), log.data());
				}
			}
		}
	}
}

//-----------------------------------------------------------------------

static bool LinkShaderProgram(const std::vector<GLuint>& stages, GLuint* program)
{
	bool success = true;
	
	*program = glCreateProgram();

	for (size_t i = 0; i < stages.size(); ++i) { glAttachShader(*program, stages[i]); }
	
	glLinkProgram(*program);

	// don't leave the compiled stages hanging around...
	for (size_t i = 0; i < stages.size(); ++i)
	{
		glDeleteShader(stages[i]);
		glDetachShader(*program, stages[i]);
	}

	// check link status and destroy the program if all is not well...
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

	return success;
}

//--------------------------------------------------------------------------------------------

class ShaderUniform
{
public:
	ShaderUniform(GLuint location, GLenum type)
		: modified(false), location(location), type(type)
	{
	}

	void Set(float value) { Cache(&value, sizeof(value), this); }
	void Set(const glm::vec2& value) { Cache(glm::value_ptr(value), sizeof(value), this); }
	void Set(const glm::vec3& value) { Cache(glm::value_ptr(value), sizeof(value), this); }
	void Set(const glm::vec4& value) { Cache(glm::value_ptr(value), sizeof(value), this); }
	void Set(const glm::mat2& value) { Cache(glm::value_ptr(value), sizeof(value), this); }
	void Set(const glm::mat3& value) { Cache(glm::value_ptr(value), sizeof(value), this); }
	void Set(const glm::mat4& value) { Cache(glm::value_ptr(value), sizeof(value), this); }

	float GetFloat() const { return *(float*)cache; }
	glm::vec2 GetVec2() { return *(glm::vec2*)cache; }
	glm::vec3 GetVec3() { return *(glm::vec3*)cache; }
	glm::vec4 GetVec4() { return *(glm::vec4*)cache; }
	glm::mat2 GetMat2() { return *(glm::mat2*)cache; }
	glm::mat3 GetMat3() { return *(glm::mat3*)cache; }
	glm::mat4 GetMat4() { return *(glm::mat4*)cache; }

	void Apply()
	{
		if (modified)
		{
			switch (type)
			{
				case GL_FLOAT:		  glUniform1fv(location, 1, (float*)cache); break;
				case GL_FLOAT_VEC2: glUniform2fv(location, 1, (float*)cache); break;
				case GL_FLOAT_VEC3: glUniform3fv(location, 1, (float*)cache); break;
				case GL_FLOAT_VEC4: glUniform4fv(location, 1, (float*)cache); break;
				case GL_FLOAT_MAT2: glUniformMatrix2fv(location, 1, GL_FALSE, (float*)cache); break;
				case GL_FLOAT_MAT3: glUniformMatrix3fv(location, 1, GL_FALSE, (float*)cache); break;
				case GL_FLOAT_MAT4: glUniformMatrix4fv(location, 1, GL_FALSE, (float*)cache); break;
				default: break;
			}
			modified = false;
		}
	}

private:
	bool modified;
	GLuint location;
	GLenum type;
	double cache[16];

	static void Cache(const void* const value, size_t size, ShaderUniform* const u)
	{
		if (0 != memcmp(u->cache, value, size))
		{
			memcpy(u->cache, value, size);
			u->modified = true;
		}
	}
};
typedef boost::shared_ptr<ShaderUniform> ShaderUniformPtr;

//--------------------------------------------------------------------------------------------

class ShaderUniformBlockElementObserver
{
public:
	virtual void Modified() = 0;
};

class ShaderUniformBlockElement
{
public:
	ShaderUniformBlockElement(ShaderUniformBlockElementObserver* const observer, void* const cache, size_t size)
		: observer(observer), cache(cache), size(size)
	{
	}

	void Set(float value) { Cache(&value, sizeof(value), this); }
	void Set(const glm::vec2& value) { Cache(glm::value_ptr(value), sizeof(value), this); }
	void Set(const glm::vec3& value) { Cache(glm::value_ptr(value), sizeof(value), this); }
	void Set(const glm::vec4& value) { Cache(glm::value_ptr(value), sizeof(value), this); }
	void Set(const glm::mat2& value) { Cache(glm::value_ptr(value), sizeof(value), this); }
	void Set(const glm::mat3& value) { Cache(glm::value_ptr(value), sizeof(value), this); }
	void Set(const glm::mat4& value) { Cache(glm::value_ptr(value), sizeof(value), this); }

	float GetFloat() const { return *(float*)cache; }
	glm::vec2 GetVec2() { return *(glm::vec2*)cache; }
	glm::vec3 GetVec3() { return *(glm::vec3*)cache; }
	glm::vec4 GetVec4() { return *(glm::vec4*)cache; }
	glm::mat2 GetMat2() { return *(glm::mat2*)cache; }
	glm::mat3 GetMat3() { return *(glm::mat3*)cache; }
	glm::mat4 GetMat4() { return *(glm::mat4*)cache; }

private:
	size_t size;
	void* const cache;
	ShaderUniformBlockElementObserver* const observer;

	static void Cache(const void* const value, size_t size, ShaderUniformBlockElement* const element)
	{
		if (0 != memcmp(element->cache, value, size))
		{
			memcpy(element->cache, value, size);
			element->observer->Modified();
		}
	}
};
typedef boost::shared_ptr<ShaderUniformBlockElement> ShaderUniformBlockElementPtr;

//--------------------------------------------------------------------------------------------

class ShaderUniformBlock : public ShaderUniformBlockElementObserver
{
public:
	ShaderUniformBlock(GLuint program, GLuint index)
		: Elements(elements), program(program), size(size), index(index), modified(false)
	{
		glGetActiveUniformBlockiv(program, index, GL_UNIFORM_BLOCK_DATA_SIZE, (GLint*)&size);
		cache.resize(size);
		
		GLint numElements;
		glGetActiveUniformBlockiv(program, index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &numElements);

		std::vector<GLuint> elementIndices(numElements);
		glGetActiveUniformBlockiv(program, index, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, (GLint*)elementIndices.data());
		
		std::vector<GLint> elementTypes(numElements);
		std::vector<GLint> elementSizes(numElements);
		std::vector<GLint> elementOffsets(numElements);
		glGetActiveUniformsiv(program, numElements, elementIndices.data(), GL_UNIFORM_TYPE, elementTypes.data());
		glGetActiveUniformsiv(program, numElements, elementIndices.data(), GL_UNIFORM_SIZE, elementSizes.data());
		glGetActiveUniformsiv(program, numElements, elementIndices.data(), GL_UNIFORM_OFFSET, elementOffsets.data());
		
		char name[256] = {0};
		for (int i = 0; i < numElements; ++i)
		{
			glGetActiveUniformName(program, elementIndices[i], sizeof(name)-1, NULL, name);
			elements[name] = boost::make_shared<ShaderUniformBlockElement>(this, cache.data() + elementOffsets[i], elementSizes[i]);
		}
	}

	virtual void Modified() { modified = true; }

	typedef std::map<std::string, ShaderUniformBlockElementPtr> ElementList;
	ElementList& Elements;

private:
	GLuint program;
	size_t size;
	GLuint index;
	bool modified;
	std::vector<GLubyte> cache;
	ElementList elements;
};
typedef boost::shared_ptr<ShaderUniformBlock> ShaderUniformBlockPtr;

//--------------------------------------------------------------------------------------------

class Shader
{
public:
	Shader(const std::string& name)
		: Uniforms(uniforms), UniformBlocks(blocks)
	{
		std::vector<GLuint> shaders;
		CompileShader(name, shaders);
		LinkShaderProgram(shaders, &program);
		FindUniforms();
	}

	virtual ~Shader() { glDeleteProgram(program); }

	typedef std::map<std::string, ShaderUniformPtr> UniformList;
	typedef std::map<std::string, ShaderUniformBlockPtr> UniformBlockList;

	UniformList& Uniforms;
	UniformBlockList& UniformBlocks;

private:
	GLuint program;
	UniformList uniforms;
	UniformBlockList blocks;

	void FindUniforms()
	{
		GLint numUniforms;
		glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUniforms);

		char name[256] = {0};
		GLint components;
		GLenum type;
		for (int i = 0; i < numUniforms; ++i)
		{
			glGetActiveUniform(program, i, sizeof(name)-1, NULL, &components, &type, name);
			const GLuint location = glGetUniformLocation(program, name);
			if (-1 != location)
			{
				uniforms[name] = boost::make_shared<ShaderUniform>(location, type);
			}
		}
	}

	void FindUniformBlocks()
	{
		GLint numBlocks;
		glGetProgramiv(program, GL_ACTIVE_UNIFORM_BLOCKS, &numBlocks);

		char name[256] = {0};
		for (int i = 0; i < numBlocks; ++i)
		{
			glGetActiveUniformBlockName(program, i, sizeof(name)-1, NULL, name);
			blocks[name] = boost::make_shared<ShaderUniformBlock>(program, i);
		}
	}
};
typedef boost::shared_ptr<Shader> ShaderPtr;

//--------------------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	ShaderPtr shader = boost::make_shared<Shader>("shaders/simple");

	shader->UniformBlocks["Common"]->Elements["CameraPos"]->Set(glm::vec3(0));

	return 0;
}
