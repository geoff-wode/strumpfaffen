#include <gl_loader/gl_loader.h>
#include <string>
#include <vector>
#include <shaders/shader.h>
#include <boost/scoped_ptr.hpp>
#include <util/files.h>
#include <util/debug.h>
#include "paramimpl.h"
#include "shaderimpl.h"

using namespace Graphics;

//--------------------------------------------------------------------------------

static void CompileShaderStages(const std::string& name, std::vector<GLuint>& stages);
static bool LinkShader(const std::vector<GLuint>& stages, GLuint& handle);

//--------------------------------------------------------------------------------

ShaderImpl::ShaderImpl()
{
}

ShaderImpl::~ShaderImpl()
{
	glDeleteProgram(handle);
}

//--------------------------------------------------------------------------------

Shader IShader::Load(const std::string& name)
{
	boost::shared_ptr<ShaderImpl> shader(new ShaderImpl());

	std::vector<GLuint> stages;
	CompileShaderStages(name, stages);

	if (LinkShader(stages, shader->handle))
	{
		// TODO: get shader parameters
	}
	else
	{
		shader.reset();
	}

	return shader;
}

//--------------------------------------------------------------------------------

const ShaderParamList& ShaderImpl::GetParams() const
{
	return params;
}

//--------------------------------------------------------------------------------

void ShaderImpl::Apply()
{
	glUseProgram(handle);
	for (size_t i = 0; i < params.size(); ++i)
	{
		params[i]->Apply();
	}
	glUseProgram(0);
}

//--------------------------------------------------------------------------------

static void CompileShaderStages(const std::string& name, std::vector<GLuint>& stages)
{
	static std::vector<char> commonSource;
	if (commonSource.empty()) { Utils::File::Load("shaders\\common.glsl", commonSource); }

	// load the names of the stages making up the shader
	std::vector<std::string> files;
	Utils::File::ListFiles("shaders\\" + name, files);
	if (!files.empty())
	{
		const char* parts[] =
		{
			commonSource.data(),
			NULL
		};

		// compile each stage...
		std::vector<GLuint> stages(files.size());
		for (size_t i = 0; i < files.size(); ++i)
		{
			std::vector<char> sourceCode;
			if (Utils::File::Load(files[i], sourceCode))
			{
				parts[1] = sourceCode.data();
				GLenum type;

				if (std::string::npos != files[i].find("vs.glsl")) { type = GL_VERTEX_SHADER; }
				else if (std::string::npos != files[i].find("gs.glsl")) { type = GL_GEOMETRY_SHADER; }
				else if (std::string::npos != files[i].find("fs.glsl")) { type = GL_FRAGMENT_SHADER; }
				else { continue; }

				stages[i] = glCreateShader(type);
				glShaderSource(stages[i], 2, parts, NULL);
				glCompileShader(stages[i]);

				GLint status;
				glGetShaderiv(stages[i], GL_COMPILE_STATUS, &status);
				if (GL_TRUE == status)
				{
					LOG("succeeded\n");
				}
				else
				{
					GLint logLength;
					glGetShaderiv(stages[i], GL_INFO_LOG_LENGTH, &logLength);
					std::vector<GLchar> log(logLength+1);
					glGetShaderInfoLog(stages[i], logLength, NULL, log.data());
					LOG("failed:\n%s\n", log.data());
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------

static bool LinkShader(const std::vector<GLuint>& stages, GLuint& handle)
{
	bool success = true;
	
	handle = glCreateProgram();

	for (size_t i = 0; i < stages.size(); ++i) { glAttachShader(handle, stages[i]); }
	
	glLinkProgram(handle);

	// don't leave the compiled stages hanging around...
	for (size_t i = 0; i < stages.size(); ++i)
	{
		glDeleteShader(stages[i]);
		glDetachShader(handle, stages[i]);
	}

	// check link status and destroy the shader if all is not well...
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

	return success;
}

//--------------------------------------------------------------------------------

static void InitParams(GLuint shader, ShaderParamList& params)
{
	GLint numParams;
	glGetProgramiv(shader, GL_ACTIVE_UNIFORMS, &numParams);

	params.resize(numParams);

	if (numParams > 0)
	{
		// Get information on each of the shader variables...

		std::vector<GLuint>	indices(numParams);
		std::vector<GLint>	blockIndices(numParams);
		std::vector<GLint>	types(numParams);
		std::vector<GLint>	nameLengths(numParams);
		std::vector<GLint>	offsets(numParams);

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
			{ GL_UNIFORM_OFFSET, offsets.data() }							// byte offset into block (will be -1 if not a block)
		};
		const size_t InfoCount = sizeof(paramInfo) / sizeof(paramInfo[0]);

		for (size_t i = 0; i < InfoCount; ++i)
		{
			glGetActiveUniformsiv(shader, numParams, indices.data(), paramInfo[i].type, paramInfo[i].data);
		}

		size_t nextParam = 0;
		for (GLint i = 0; i < numParams; ++i)
		{
			std::vector<char> name(nameLengths[i]);
			glGetActiveUniformName(shader, i, nameLengths[i], NULL, name.data());

			if (-1 == blockIndices[i])
			{
				ShaderParamPtr p(new ShaderParamImpl(name.data(), types[i], glGetUniformLocation(shader, name.data())));
				params[i] = p;
			}
			else
			{
			}
		}
	}
}
