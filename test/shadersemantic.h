#if ! defined(__SHADER_SEMANTIC__)
#define __SHADER_SEMANTIC__

#include <string>
#include <gl_loader/gl_loader.h>

struct ShaderSemantic
{
	ShaderSemantic()
		: attribIndex(-1),
			elementType(0),
			elementCount(0),
			normalise(false)
	{
	}

	ShaderSemantic(GLuint attribIndex, GLenum elementType, int elementCount, bool normalise)
		: attribIndex(attribIndex),
			elementType(elementType),
			elementCount(elementCount),
			normalise(normalise)
	{
	}

	GLuint	attribIndex;
	GLuint	elementType;
	GLenum	elementCount;
	bool		normalise;

	static const ShaderSemantic* const Get(const std::string& semantic);
};

#endif // __SHADER_SEMANTIC__
