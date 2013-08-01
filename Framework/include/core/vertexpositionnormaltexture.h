#if ! defined(__VERTEX_POSITION_NORMAL_TEXTURE__)
#define __VERTEX_POSITION_NORMAL_TEXTURE__

#include <glm/glm.hpp>

namespace Core
{
	struct VertexPositionNormalTexture
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoord;

		static size_t Stride() { return sizeof(VertexPositionNormalTexture); }
	};
}

#endif // __VERTEX_POSITION_NORMAL_TEXTURE__
