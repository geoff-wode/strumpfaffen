#if ! defined(__VERTEX_POSITION_H__)
#define __VERTEX_POSITION_H__

#include <glm/glm.hpp>
#include <vertextypes/vertextype.h>

namespace Framework
{
	class VertexPosition : public IVertexType
	{
	public:
		glm::vec3 Position;

		virtual const VertexDeclaration& GetVertexDeclaration() const;
	};

}

#endif // __VERTEX_POSITION_H__
