#include <util/debug.h>
#include "shaderimpl.h"

using namespace Shaders;

//----------------------------------------------------------------------------

ShaderClassImpl::ShaderClassImpl(GLuint handle)
	: handle(handle)
{
	GetParameters();
}

//-----------------------------------------------------------------------------
ShaderClassImpl::~ShaderClassImpl()
{
	glDeleteProgram(handle);
	handle = 0;
}

//-----------------------------------------------------------------------------
ShaderParameter* const ShaderClassImpl::GetParameter(const std::string& name) const
{
	for (size_t i = 0; i < parameters.size(); ++i)
	{
		if (name == parameters[i].name)
		{
			return (ShaderParameter* const)&parameters[i];
		}
	}

	LOG("unknown shader parameter: %s\n", name.c_str());
	return NULL;
}

//-----------------------------------------------------------------------------
void ShaderClassImpl::Apply()
{
	glUseProgram(handle);
	for (size_t i = 0; i < parameters.size(); ++i)
	{
		parameters[i].Update();
	}
}

//-----------------------------------------------------------------------------
void ShaderClassImpl::GetParameters()
{
	GLint numParams;
	glGetProgramiv(handle, GL_ACTIVE_UNIFORMS, &numParams);
	parameters.resize(numParams);

	if (numParams > 0)
	{
		GLint maxNameLength;
		glGetProgramiv(handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLength);
		std::vector<char>	paramName(maxNameLength);
	
		std::vector<GLuint>	indices(numParams);
		std::vector<GLint>	blockIndices(numParams);
		std::vector<GLint>	types(numParams);

		for (GLint i = 0; i < numParams; ++i ) { indices[i] = i; }

		glGetActiveUniformsiv(handle, numParams, indices.data(), GL_UNIFORM_BLOCK_INDEX, blockIndices.data());
		glGetActiveUniformsiv(handle, numParams, indices.data(), GL_UNIFORM_TYPE, types.data());

		unsigned int index = 0;
		for (GLint i = 0; i < numParams; ++i)
		{
			if (-1 == blockIndices[i])
			{
				glGetActiveUniformName(handle, i, maxNameLength, NULL, paramName.data());

				parameters[i].name = paramName.data();
				parameters[i].location = glGetUniformLocation(handle, paramName.data());
				parameters[i].type = types[i];
				++index;
			}
		}
	}
}

//-----------------------------------------------------------------------------
