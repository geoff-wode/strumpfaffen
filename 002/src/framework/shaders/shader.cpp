#include <framework/shaders/shader.h>
#include <vector>
#include <framework/files.h>
#include <framework/debug.h>
#include <framework/scenegraph/scene.h>

using namespace Framework;

//-----------------------------------------------------------------------

static void Compile(const std::string& name, std::vector<GLuint>& stages);
static bool Link(const std::vector<GLuint>& stages, GLuint* shader);
static void FindUniforms(GLuint program, ShaderUniformMap& uniforms);
static void FindUniformBlocks(GLuint program, Device* const device);
static void FindVertexAttributes(GLuint program, ShaderVertexAttributeList& attrs);

//-----------------------------------------------------------------------

Shader::Shader(const std::string& name, Device* const device)
{
	std::vector<GLuint> stages;
	Compile(name, stages);
	
	if (Link(stages, &program))
	{
		FindVertexAttributes(program, vertexAttrs);
		FindUniforms(program, uniforms);
		FindUniformBlocks(program, device);
	}
}

Shader::~Shader()
{
	glDeleteProgram(program);
}

void Shader::Enable()
{
	glUseProgram(program);
}

void Shader::Disable()
{
	glUseProgram(0);
}

void Shader::Apply()
{
	for (ShaderUniformMap::iterator i = uniforms.begin(); i != uniforms.end(); ++i)
	{
		i->second->Apply();
	}
}

//-----------------------------------------------------------------------

static void Compile(const std::string& name, std::vector<GLuint>& stages)
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

static bool Link(const std::vector<GLuint>& stages, GLuint* program)
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

//-----------------------------------------------------------------------

static void FindUniforms(GLuint program, ShaderUniformMap& uniforms)
{
	// get the number of uniforms...
	GLint numParams;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numParams);

	if (numParams > 0)
	{
		// Get information on each of the shader variables...

		std::vector<GLuint>	indices(numParams);
		for (GLint i = 0; i < numParams; ++i ) { indices[i] = i; }

		std::vector<GLint>	blockIndices(numParams);
		std::vector<GLint>	types(numParams);
		glGetActiveUniformsiv(program, numParams, indices.data(), GL_UNIFORM_BLOCK_INDEX, blockIndices.data());
		glGetActiveUniformsiv(program, numParams, indices.data(), GL_UNIFORM_TYPE, types.data());

		for (GLint i = 0; i < numParams; ++i)
		{
			char name[256];
			glGetActiveUniformName(program, i, sizeof(name)-1, NULL, name);

			if (-1 == blockIndices[i])
			{
				GLuint size;
				switch (types[i])
				{
				case GL_FLOAT:		  size = sizeof(float); break;
				case GL_FLOAT_VEC2: size = sizeof(glm::vec2); break;
				case GL_FLOAT_VEC3: size = sizeof(glm::vec3); break;
				case GL_FLOAT_VEC4: size = sizeof(glm::vec4); break;
				case GL_FLOAT_MAT2: size = sizeof(glm::mat2); break;
				case GL_FLOAT_MAT3: size = sizeof(glm::mat3); break;
				case GL_FLOAT_MAT4: size = sizeof(glm::mat4); break;
				default: continue; break;
				}

				ShaderUniformPtr u(new ShaderUniform(name, types[i], glGetUniformLocation(program, name)));

				uniforms[name] = u;
			}
		}
	}
}

//-----------------------------------------------------------------------
static void FindUniformBlocks(GLuint program, Device* const device)
{
	GLint numBlocks;
	glGetProgramiv(program, GL_ACTIVE_UNIFORM_BLOCKS, &numBlocks);

	for (GLint i = 0; i < numBlocks; ++i)
	{
		char name[256];
		glGetActiveUniformBlockName(program, i, sizeof(name)-1, NULL, name);

		GLint blockSize;
		GLint numUniformsInBlock;
		glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
		glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &numUniformsInBlock);

		std::vector<GLuint> blockUniformIndices(numUniformsInBlock);
		std::vector<GLint> types(numUniformsInBlock);
		std::vector<GLint> offsets(numUniformsInBlock);

		glGetActiveUniformBlockiv(program, i, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, (GLint*)blockUniformIndices.data());
		glGetActiveUniformsiv(program, blockUniformIndices.size(), blockUniformIndices.data(), GL_UNIFORM_TYPE, types.data());
		glGetActiveUniformsiv(program, blockUniformIndices.size(), blockUniformIndices.data(), GL_UNIFORM_OFFSET, offsets.data());

		if (device->uniformBufferBindings.find(name) != device->uniformBufferBindings.end())
		{
			const GLuint uniformBlockIndex = glGetUniformBlockIndex(program, name);
			const GLuint bufferBinding = device->uniformBufferBindings[name];
			glUniformBlockBinding(program, uniformBlockIndex, bufferBinding);
		}
	}
}

//-----------------------------------------------------------------------

static void FindVertexAttributes(GLuint program, ShaderVertexAttributeList& attrs)
{
	GLint numAttrs;
	glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &numAttrs);

	attrs.reserve(numAttrs);

	for (GLint i = 0; i < numAttrs; ++i)
	{
		char attrName[256];
		ShaderVertexAttribute attr;

		glGetActiveAttrib(program, i, sizeof(attrName)-1, NULL, (GLint*)&attr.components, &attr.type, attrName);

		// ignore the built-in attributes with a 'gl_' prefix...
		if (0 != strcmp("gl_", attrName))
		{
			attr.name = attrName;
			attr.location = glGetAttribLocation(program, attrName);

			attrs.push_back(attr);
		}
	}
}
