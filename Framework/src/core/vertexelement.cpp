#include <glm/glm.hpp>
#include <core/vertexelement.h>

using namespace Core;

size_t VertexElementComponents(const VertexElement& element)
{
	switch (element.Type)
	{
	case VertexElementType::Float:				return 1; break;
	case VertexElementType::FloatVector2: return 2; break;
	case VertexElementType::FloatVector3: return 3; break;
	case VertexElementType::FloatVector4: return 4; break;
	default: break;
	}
	return 0;
}
