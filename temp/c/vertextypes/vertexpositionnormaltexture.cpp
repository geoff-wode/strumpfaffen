#include <vertextypes/vertexpositionnormaltexture.h>

using namespace Framework;

const VertexDeclaration& VertexPositionNormalTexture::GetVertexDeclaration() const
{
	static const VertexElement elements[] =
	{
		{ "Position", VertexElement::DataType::FloatVector3, offsetof(VertexPositionNormalTexture, Position) },
		{ "Normal", VertexElement::DataType::FloatVector3, offsetof(VertexPositionNormalTexture, Normal) },
		{ "TexCoord", VertexElement::DataType::FloatVector3, offsetof(VertexPositionNormalTexture, TextureCoord) }
	};
	static const size_t numElements = sizeof(elements) / sizeof(elements[0]);
	static const VertexDeclaration decl(numElements, elements);

	return decl;
}
