#if ! defined(__VERTEX_ELEMENT_H__)
#define __VERTEX_ELEMENT_H__

#include <string>
#include <gl_loader/gl_loader.h>

namespace Core
{
	namespace VertexElementType
	{
		enum Enum
		{
			Float				 = GL_FLOAT,
			FloatVector2 = GL_FLOAT_VEC2,
			FloatVector3 = GL_FLOAT_VEC3,
			FloatVector4 = GL_FLOAT_VEC4
		};
	};

	struct VertexElement
	{
		VertexElement(const std::string& name, VertexElementType::Enum type, size_t offset)
			: Name(name), Type(type), Offset(offset)
		{
		}

		const std::string			Name;
		const VertexElementType::Enum	Type;
		const size_t					Offset;
	};

	size_t VertexElementComponents(const VertexElement& element);

	typedef const VertexElement* const VertexElementArray;
}

#endif // __VERTEX_ELEMENT_H__
