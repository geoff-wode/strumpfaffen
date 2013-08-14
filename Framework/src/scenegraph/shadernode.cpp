#include <util/debug.h>
#include <scenegraph/scene.h>
#include <scenegraph/shadernode.h>
#include <util/files.h>
#include <util/debug.h>
#include <gl_loader/gl_loader.h>
#include <shaders/uniform.h>

using namespace SceneGraph;
using namespace Graphics;

//--------------------------------------------------------------------------------

static void Compile(const std::string& name, std::vector<GLuint>& stages);
static bool Link(const std::vector<GLuint>& stages, GLuint* program);
static void LoadUniforms(GLuint program, Graphics::ShaderUniformList& uniforms);
static void LoadUniformBlocks(GLuint program, Scene* const scene);

//--------------------------------------------------------------------------------

boost::shared_ptr<ShaderNode> ShaderNode::Create(const std::string& filename)
{
	boost::shared_ptr<ShaderNode> program(new ShaderNode(filename));

	return program;
}

ShaderNode::ShaderNode(const std::string& filename)
	: filename(filename), program(0)
{
}

ShaderNode::~ShaderNode()
{
	if (program) { glDeleteProgram(program); }
	program = 0;
}

void ShaderNode::LoadResources(Scene* const scene)
{
	std::vector<GLuint> programStages;
	Compile(filename, programStages);
	Link(programStages, &program);
	LoadUniforms(program, uniforms);
	LoadUniformBlocks(program, scene);
}

void ShaderNode::Render(Scene* const scene)
{
	glUseProgram(program);

	for (size_t i = 0; i < uniforms.size(); ++i)
	{
		if (uniforms[i].changed)
		{
			switch (uniforms[i].type)
			{
			case GL_FLOAT:		  glUniform1fv(uniforms[i].location, 1, (float*)uniforms[i].cache); break;
			case GL_FLOAT_VEC2: glUniform2fv(uniforms[i].location, 1, (float*)uniforms[i].cache); break;
			case GL_FLOAT_VEC3: glUniform3fv(uniforms[i].location, 1, (float*)uniforms[i].cache); break;
			case GL_FLOAT_VEC4: glUniform4fv(uniforms[i].location, 1, (float*)uniforms[i].cache); break;
			case GL_FLOAT_MAT2: glUniformMatrix2fv(uniforms[i].location, 1, GL_FALSE, (float*)uniforms[i].cache); break;
			case GL_FLOAT_MAT3: glUniformMatrix3fv(uniforms[i].location, 1, GL_FALSE, (float*)uniforms[i].cache); break;
			case GL_FLOAT_MAT4: glUniformMatrix4fv(uniforms[i].location, 1, GL_FALSE, (float*)uniforms[i].cache); break;
			default: break;
			}
			uniforms[i].changed = false;
		}
	}
}

//-------------------------------------------------------------------------------------------

static void Compile(const std::string& name, std::vector<GLuint>& stages)
{
	static std::vector<char> commonSource;
	if (commonSource.empty()) { Utils::File::Load("programs/common.glsl", commonSource); }

	// load the names of the stages making up the program
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

//-------------------------------------------------------------------------------------------

static void LoadUniforms(GLuint program, Graphics::ShaderUniformList& uniforms)
{
	// get the number of uniforms...
	GLint numParams;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numParams);

	if (numParams > 0)
	{
		// Get information on each of the program variables...

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
			glGetActiveUniformsiv(program, numParams, indices.data(), paramInfo[i].type, paramInfo[i].data);
		}

		for (GLint i = 0; i < numParams; ++i)
		{
			char nameBuffer[256];
			glGetActiveUniformName(program, i, nameLengths[i], NULL, nameBuffer);

			if (-1 == blockIndices[i])
			{
				std::string name(nameBuffer, nameLengths[i]);
				ShaderUniform u(name, types[i], glGetUniformLocation(program, nameBuffer));
				uniforms.push_back(u);
			}
		}
	}
}

//-------------------------------------------------------------------------------------------

static void LoadUniformBlocks(GLuint program, Scene* const scene)
{
	const GLuint blockIndex = glGetUniformBlockIndex(program, "CommonShaderVarsBlock");

	if (-1 != blockIndex)
	{
		for (size_t i = 0; i < scene->UniformBlocks.size(); ++i)
		{
			if ("CommonShaderVarsBlock" == scene->UniformBlocks[i].name)
			{
				glUniformBlockBinding(program, blockIndex, scene->UniformBlocks[i].binding);
				return;
			}
		}
	}
}
