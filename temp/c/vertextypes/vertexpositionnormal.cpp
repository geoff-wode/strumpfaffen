#include <vertextypes/vertexpositionnormal.h>

using namespace Framework;

const VertexDeclaration& VertexPositionNormal::GetVertexDeclaration() const
{
	static const VertexElement elements[] =
	{
		{ "Position", VertexElement::DataType::FloatVector3, offsetof(VertexPositionNormal, Position) },
		{ "Normal", VertexElement::DataType::FloatVector3, offsetof(VertexPositionNormal, Normal) }
	};
	static const size_t numElements = sizeof(elements) / sizeof(elements[0]);
	static const VertexDeclaration decl(numElements, elements);

	return decl;
}
