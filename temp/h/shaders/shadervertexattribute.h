#if ! defined(__SHADER_VERTEX_ATTRIBUTE_H__)
#define __SHADER_VERTEX_ATTRIBUTE_H__

#include <gl_loader/gl_loader.h>

namespace Shaders
{

	struct ShaderVertexAttribute
	{
		std::string	Name;
		GLuint			Location;
		GLenum			Type;
	};

}

#endif // __SHADER_VERTEX_ATTRIBUTE_H__
