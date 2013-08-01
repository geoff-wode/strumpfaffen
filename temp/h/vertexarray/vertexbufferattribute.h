// A vertex attribute defines the position, size and type of one element in a vertex data stream
// (e.g. the position).

#if ! defined(__VERTEX_BUFFER_ATTRIBUTE_H__)
#define __VERTEX_BUFFER_ATTRIBUTE_H__

#include <string>
#include <gl_loader/gl_loader.h>

namespace VertexArrays
{

	struct VertexBufferAttribute
	{
		struct DataType
		{
			enum Enum
			{
				Int,
				IntVector2,
				IntVector3,
				IntVector4,
				Float,
				FloatVector2,
				FloatVector3,
				FloatVector4,
				HalfFloat,
				HalfFloatVector2,
				HalfFloatVector4
			};
		};

		std::string			Name;
		DataType::Enum	Type;
		size_t					Offset;
	};

	size_t BufferAttrTypeComponentCount(VertexBufferAttribute::DataType::Enum type);
	size_t BufferAttrTypeElementSize(VertexBufferAttribute::DataType::Enum type);
	GLenum BufferAttrTypeGLType(VertexBufferAttribute::DataType::Enum type);
}

#endif // __VERTEX_BUFFER_ATTRIBUTE_H__
