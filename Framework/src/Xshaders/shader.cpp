// Implements shader creation and associated support functions.

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <gl_loader/gl_loader.h>
#include <buffers/buffer.h>
#include <util/debug.h>
#include <util/files.h>
#include <shaders/shader.h>
#include <shaders/uniform.h>
#include "shaderimpl.h"
#include "uniformblockimpl.h"
#include "uniformimpl.h"
#include <util/debug.h>

using namespace Graphics;

//-------------------------------------------------------------------------------------------

static void Compile(const std::string& name, std::vector<GLuint>& stages);
static bool Link(const std::vector<GLuint>& stages, GLuint* shader);
static void GetUniformBlock(struct ShaderImpl* const shader, GLuint blockIndex);
static void GetUniforms(struct ShaderImpl* const shader);
static void ListUniformBlocks(const std::vector<UniformBlockImpl>& blocks);
static void ListUniforms(const std::vector<UniformImpl>& uniforms, const char* indent = "  ");

//-------------------------------------------------------------------------------------------

Shader Graphics::LoadShader(const std::string& name)
{
	boost::shared_ptr<ShaderImpl> shader(new ShaderImpl());

	std::vector<GLuint> shaderStages;
	Compile(name, shaderStages);
	if (Link(shaderStages, &shader->handle))
	{
		GetUniforms(shader.get());
		LOG("shader %s has %d uniforms:\n", name.c_str(), shader->uniforms.size());
		ListUniforms(shader->uniforms);
		LOG("shader %s has %d uniform blocks:\n", name.c_str(), shader->blocks.size());
		ListUniformBlocks(shader->blocks);
	}
	else
	{
		shader.reset();
	}

	return shader;
}

static void ListUniformBlocks(const std::vector<UniformBlockImpl>& blocks)
{
	for (size_t i = 0; i < blocks.size(); ++i)
	{
		LOG("  block #%d: %s (%d uniforms)\n", i, blocks[i].name.c_str(), blocks[i].uniforms.size());
		ListUniforms(blocks[i].uniforms, "    ");
	}
}
static void ListUniforms(const std::vector<UniformImpl>& uniforms, const char* indent)
{
	for (size_t i = 0; i < uniforms.size(); ++i)
	{
		LOG("%suniform #%d: %s\n", indent, i, uniforms[i].name.c_str());
	}
}

//-------------------------------------------------------------------------------------------

static void Compile(const std::string& name, std::vector<GLuint>& stages)
{
	static std::vector<char> commonSource;
	if (commonSource.empty()) { Utils::File::Load("shaders/common.glsl", commonSource); }

	// load the names of the stages making up the shader
	std::vector<std::string> files;
	Utils::File::ListFiles(name, files);
	if (!files.empty())
	{
		const char* parts[] = { commonSource.data(), NULL };
		GLint lengths[] = { commonSource.size(), 0 };

		// compile each stage...
		std::vector<GLuint> stages(files.size());
		for (size_t i = 0; i < files.size(); ++i)
		{
			std::vector<char> sourceCode;
			if (Utils::File::Load(files[i], sourceCode))
			{
				parts[1] = sourceCode.data();
				lengths[1] = sourceCode.size();

				if (std::string::npos != files[i].find("vs.glsl")) { stages[i] = glCreateShader(GL_VERTEX_SHADER); }
				else if (std::string::npos != files[i].find("gs.glsl")) { stages[i] = glCreateShader(GL_GEOMETRY_SHADER); }
				else if (std::string::npos != files[i].find("fs.glsl")) { stages[i] = glCreateShader(GL_FRAGMENT_SHADER); }
				else { continue; }

				glShaderSource(stages[i], 2, parts, lengths);
				glCompileShader(stages[i]);

				GLint status;
				glGetShaderiv(stages[i], GL_COMPILE_STATUS, &status);
				if (GL_TRUE == status)
				{
					LOG("%s compiled ok\n", files[i].c_str());
				}
				else
				{
					GLint logLength;
					glGetShaderiv(stages[i], GL_INFO_LOG_LENGTH, &logLength);
					std::vector<GLchar> log(logLength+1);
					glGetShaderInfoLog(stages[i], logLength, NULL, log.data());
					LOG("%s failed to compile:\n%s\n", files[i].c_str(), log.data());
				}
			}
		}
	}
}

//-------------------------------------------------------------------------------------------

static bool Link(const std::vector<GLuint>& stages, GLuint* shader)
{
	bool success = true;
	
	*shader = glCreateProgram();

	for (size_t i = 0; i < stages.size(); ++i) { glAttachShader(*shader, stages[i]); }
	
	glLinkProgram(*shader);

	// don't leave the compiled stages hanging around...
	for (size_t i = 0; i < stages.size(); ++i)
	{
		glDeleteShader(stages[i]);
		glDetachShader(*shader, stages[i]);
	}

	// check link status and destroy the shader if all is not well...
	GLint status;
	glGetProgramiv(*shader, GL_LINK_STATUS, &status);
	if (GL_FALSE == status)
	{
		success = false;
		GLint logLength;
		glGetProgramiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<GLchar> log(logLength+1);
		glGetProgramInfoLog(*shader, logLength, NULL, log.data());
		LOG("%s\n", log.data());
	}

	return success;
}

//-------------------------------------------------------------------------------------------

static void GetUniforms(ShaderImpl* const shader)
{
	// get the number of uniforms...
	GLint numParams;
	glGetProgramiv(shader->handle, GL_ACTIVE_UNIFORMS, &numParams);

	if (numParams > 0)
	{
		// Get information on each of the shader variables...

		std::vector<GLuint>	indices(numParams);
		std::vector<GLint>	blockIndices(numParams);
		std::vector<GLint>	types(numParams);
		std::vector<GLint>	nameLengths(numParams);

		for (GLint i = 0; i < numParams; ++i ) { indices[i] = i; }

		struct ParamInfo
		{
			GLenum type;
			GLint* data;
		} paramInfo [] =
		{
			{ GL_UNIFORM_TYPE, types.data() },								// data type
			{ GL_UNIFORM_NAME_LENGTH, nameLengths.data() },		// length of the name
			{ GL_UNIFORM_BLOCK_INDEX, blockIndices.data() },	// block index (will be -1 if not a block)
		};
		const size_t InfoCount = sizeof(paramInfo) / sizeof(paramInfo[0]);

		for (size_t i = 0; i < InfoCount; ++i)
		{
			glGetActiveUniformsiv(shader->handle, numParams, indices.data(), paramInfo[i].type, paramInfo[i].data);
		}

		for (GLint i = 0; i < numParams; ++i)
		{
			std::vector<char> name(nameLengths[i]);
			glGetActiveUniformName(shader->handle, i, nameLengths[i], NULL, name.data());

			if (-1 == blockIndices[i])
			{
				UniformImpl u;
				u.type = types[i];
				u.name = name.data();
				u.location = glGetUniformLocation(shader->handle, name.data());
				shader->uniforms.push_back(u);
			}
			else
			{
				GetUniformBlock(shader, blockIndices[i]);
			}
		}
	}
}

//-------------------------------------------------------------------------------------------

static void GetUniformBlock(ShaderImpl* const shader, GLuint blockIndex)
{
	static GLuint nextBinding = 0;
	static std::map<std::string,UniformBlockImpl*> bufferBindings;

	std::vector<char> blockName(256);
	glGetActiveUniformBlockName(shader->handle, blockIndex, blockName.size(), NULL, blockName.data());

	// If this block has not already been seen, create the buffer, bind it & fill out the uniforms it contains...
	const std::map<std::string,UniformBlockImpl*>::iterator i = bufferBindings.find(blockName.data());
	if (bufferBindings.end() != i)
	{
		// Create and bind the block's buffer to the next available slot...
		UniformBlockImpl block;
		block.bindingPoint = nextBinding;
		++nextBinding;
		GLint blockSize;
		glGetActiveUniformBlockiv(shader->handle, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

		block.name = blockName.data();

		block.buffer = IUniformBuffer::Create(blockSize, block.bindingPoint, BufferUsageHint::Stream);
		// Also create the CPu-side cache...
		block.cache.resize(blockSize);

		// Get the individual uniforms in the block...
		GLint numUniforms;
		glGetActiveUniformBlockiv(shader->handle, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &numUniforms);

		std::vector<GLuint> indices(numUniforms);
		std::vector<GLenum> types(numUniforms);
		std::vector<GLint> offsets(numUniforms);
		std::vector<GLint> nameLengths(numUniforms);

		glGetActiveUniformBlockiv(shader->handle, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, (GLint*)indices.data());

		struct UniformInfo
		{
			GLenum type;
			GLint* data;
		} info[] =
		{
			{ GL_UNIFORM_NAME_LENGTH, nameLengths.data() },
			{ GL_UNIFORM_TYPE, (GLint*)types.data() },
			{ GL_UNIFORM_OFFSET, offsets.data() }
		};
		const size_t infoCount = sizeof(info) / sizeof(info[0]);
		for (size_t i = 0; i < infoCount; ++i)
		{
			glGetActiveUniformsiv(shader->handle, indices.size(), indices.data(), info[i].type, info[i].data);
		}

		// Fill out the list of uniforms...
		for (GLint i = 0; i < numUniforms; ++i)
		{
			UniformImpl uniform;

			std::vector<char> name(nameLengths[i]);
			glGetActiveUniformName(shader->handle, indices[i], nameLengths[i], NULL, name.data());
			uniform.name = name.data();

			uniform.location = glGetUniformLocation(shader->handle, uniform.name.c_str());

			switch (types[i])
			{
			case GL_FLOAT:		  uniform.cache.resize(sizeof(float)); break;
			case GL_FLOAT_VEC2: uniform.cache.resize(sizeof(glm::vec2)); break;
			case GL_FLOAT_VEC3: uniform.cache.resize(sizeof(glm::vec3)); break;
			case GL_FLOAT_VEC4: uniform.cache.resize(sizeof(glm::vec4)); break;
			case GL_FLOAT_MAT2: uniform.cache.resize(sizeof(glm::mat2)); break;
			case GL_FLOAT_MAT3: uniform.cache.resize(sizeof(glm::mat3)); break;
			case GL_FLOAT_MAT4: uniform.cache.resize(sizeof(glm::mat4)); break;
			}

			block.uniforms.push_back(uniform);
		}

		shader->blocks.push_back(block);
		bufferBindings[blockName.data()] = &shader->blocks[shader->blocks.size()-1];
	}
}
