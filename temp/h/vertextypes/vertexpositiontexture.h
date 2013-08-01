#if ! defined(__VERTEX_POSITION_TEXTURE_H__)
#define __VERTEX_POSITION_TEXTURE_H__

#include <glm/glm.hpp>
#include <vertextypes/vertextype.h>
#include <vertextypes/vertexdeclaration.h>

namespace Framework
{

	class VertexPositionTexture : public IVertexType
	{
	public:
		glm::vec3 Position;
		glm::vec2 TextureCoord;

		virtual const VertexDeclaration& GetVertexDeclaration() const;
	};

}

#endif // __VERTEX_POSITION_TEXTURE_H__
