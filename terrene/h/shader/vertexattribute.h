#if ! defined(__SHADER_VERTEX_ATTRIBUTE_H__)
#define __SHADER_VERTEX_ATTRIBUTE_H__

#include <string>
#include <gl_loader/gl_loader.h>

namespace Terrene
{
	namespace Shaders
	{

		struct VertexAttribute
		{
			VertexAttribute(const std::string& name, unsigned int location, GLenum type)
				: Name(name), Location(location), Type(type)
			{
			}

			const std::string		Name;
			const unsigned int	Location;
			const GLenum				Type;
		};

	}
}

#endif // __SHADER_VERTEX_ATTRIBUTE_H__
