#include <vertextypes.h>

using namespace Terrene;

//---------------------------------------------------------------------------------------------------------

namespace PositionTexture
{
	const VertexArrayElement Elements[] =
	{
		VertexArrayElement("Position", GL_FLOAT, 3, sizeof(glm::vec3), offsetof(VertexPositionTexture, Position)),
		VertexArrayElement("TexCoord0", GL_FLOAT, 2, sizeof(glm::vec2), offsetof(VertexPositionTexture, TexCoord))
	};
	const unsigned int NumElements = sizeof(Elements) / sizeof(Elements[0]);
}

//---------------------------------------------------------------------------------------------------------

namespace PositionNormalTexture
{
	const VertexArrayElement Elements[] =
	{
		VertexArrayElement("Position", GL_FLOAT, 3, sizeof(glm::vec3), offsetof(VertexPositionNormalTexture, Position)),
		VertexArrayElement("Normal", GL_FLOAT, 3, sizeof(glm::vec3), offsetof(VertexPositionNormalTexture, Normal)),
		VertexArrayElement("TexCoord0", GL_FLOAT, 2, sizeof(glm::vec2), offsetof(VertexPositionNormalTexture, TexCoord))
	};
	const unsigned int NumElements = sizeof(Elements) / sizeof(Elements[0]);
}

//---------------------------------------------------------------------------------------------------------

const VertexDeclaration VertexPositionTexture::VertexDecl(PositionTexture::NumElements, PositionTexture::Elements);
const VertexDeclaration VertexPositionNormalTexture::VertexDecl(PositionNormalTexture::NumElements, PositionNormalTexture::Elements);
