#include <core/vertexdeclaration.h>
#include <glm/glm.hpp>

using namespace Core;

//-----------------------------------------------------------------

static size_t ComputeStride(size_t elementCount, VertexElementArray elements);

//-----------------------------------------------------------------

VertexDeclaration::VertexDeclaration(size_t numElements, const VertexElement* const elements)
	: elementCount(numElements),
		elementList(elements),
		vertexStride(ComputeStride(elementCount, elementList))
{
}

size_t VertexDeclaration::VertexStride() const
{
	return vertexStride;
}

size_t VertexDeclaration::GetElementCount() const
{
	return elementCount;
}

VertexElementArray VertexDeclaration::GetElements()
{
	return elementList;
}

//-----------------------------------------------------------------

static size_t ComputeStride(size_t elementCount, VertexElementArray elements)
{
	size_t stride = 0;
	for (size_t i = 0; i < elementCount; ++i)
	{
		switch (elements[i].Type)
		{
		case VertexElementType::Float:			  stride += sizeof(float); break;
		case VertexElementType::FloatVector2: stride += sizeof(glm::vec2); break;
		case VertexElementType::FloatVector3: stride += sizeof(glm::vec3); break;
		case VertexElementType::FloatVector4: stride += sizeof(glm::vec4); break;
		default: break;
		}
	}
	return stride;
}
