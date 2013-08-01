#include <vertexarray/vertexbufferattribute.h>
#include <glm/glm.hpp>
#include <glm/gtc/half_float.hpp>

struct ComponentInfoStruct
{
	size_t componentCount;
	size_t elementSize;
	GLenum glType;
};

static ComponentInfoStruct ComponentInfo[] =
{
	{ 1, sizeof(int),					GL_INT				},	// Int
	{ 2, sizeof(glm::ivec2),	GL_INT_VEC2		},	// IntVector2
	{ 3, sizeof(glm::ivec3),	GL_INT_VEC3		},	// IntVector3
	{ 4, sizeof(glm::ivec4),	GL_INT_VEC4		},	// IntVector4
	{ 1, sizeof(float),				GL_FLOAT			},	// Float
	{ 2, sizeof(glm::vec2),		GL_FLOAT_VEC2	},	// FloatVector2
	{ 3, sizeof(glm::vec3),		GL_FLOAT_VEC2	},	// FloatVector3
	{ 4, sizeof(glm::vec4),		GL_FLOAT_VEC3	},	// FloatVector4
	{ 1, sizeof(float) / 2,		GL_HALF_FLOAT	},	// HalfFloat
	{ 2, sizeof(glm::hvec2),	GL_HALF_FLOAT	},	// HalfFloatVector2
	{ 4, sizeof(glm::hvec4),	GL_HALF_FLOAT	}		// HalfFloatVector4
};

//-----------------------------------------------------------------

size_t VertexArrays::BufferAttrTypeComponentCount(VertexBufferAttribute::DataType::Enum type)
{
	return ComponentInfo[type].componentCount;
}

size_t VertexArrays::BufferAttrTypeElementSize(VertexBufferAttribute::DataType::Enum type)
{
	return ComponentInfo[type].elementSize;
}

GLenum VertexArrays::BufferAttrTypeGLType(VertexBufferAttribute::DataType::Enum type)
{
	return ComponentInfo[type].glType;
}
