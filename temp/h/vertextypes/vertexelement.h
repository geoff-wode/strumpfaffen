#if ! defined(__VERTEX_ELEMENT_H__)
#define __VERTEX_ELEMENT_H__

#include <string>
#include <gl_loader/gl_loader.h>

namespace Framework
{
	struct VertexElement
	{
		struct DataType
		{
			enum Enum
			{
				Float				 = GL_FLOAT,
				FloatVector2 = GL_FLOAT_VEC2,
				FloatVector3 = GL_FLOAT_VEC3,
				FloatVector4 = GL_FLOAT_VEC4
			};
		};

		std::string			Name;
		DataType::Enum	Type;
		size_t					Offset;

		size_t Components() const;
	};


	typedef const VertexElement* const VertexElementArray;
}

#endif // __VERTEX_ELEMENT_H__
