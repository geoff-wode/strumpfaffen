#include <vertextypes/vertexpositiontexture.h>

using namespace Framework;

const VertexDeclaration& VertexPositionTexture::GetVertexDeclaration() const
{
	static const VertexElement elements[] =
	{
		{ "Position", VertexElement::DataType::FloatVector3, offsetof(VertexPositionTexture, Position) },
		{ "Normal", VertexElement::DataType::FloatVector3, offsetof(VertexPositionTexture, TextureCoord) }
	};
	static const size_t numElements = sizeof(elements) / sizeof(elements[0]);
	static const VertexDeclaration decl(numElements, elements);

	return decl;
}
