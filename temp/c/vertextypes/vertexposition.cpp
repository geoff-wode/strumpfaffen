#include <vertextypes/vertexposition.h>

using namespace Framework;

const VertexDeclaration& VertexPosition::GetVertexDeclaration() const
{
	static const VertexElement elements[] =
	{
		{ "Position", VertexElement::DataType::FloatVector3, offsetof(VertexPosition, Position) }
	};
	static const size_t numElements = sizeof(elements) / sizeof(elements[0]);
	static const VertexDeclaration decl(numElements, elements);

	return decl;
}
