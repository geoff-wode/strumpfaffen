#if ! defined(__VERTEX_POSITION_NORMAL_H__)
#define __VERTEX_POSITION_NORMAL_H__

#include <glm/glm.hpp>
#include <vertextypes/vertextype.h>
#include <vertextypes/vertexdeclaration.h>

namespace Framework
{

	class VertexPositionNormal : public IVertexType
	{
	public:
		glm::vec3 Position;
		glm::vec3 Normal;

		virtual const VertexDeclaration& GetVertexDeclaration() const;
	};

}

#endif // __VERTEX_POSITION_NORMAL_H__
