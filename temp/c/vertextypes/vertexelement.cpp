#include <vertextypes/vertexelement.h>
#include <glm/glm.hpp>

size_t Framework::VertexElement::Components() const
{
	switch (Type)
	{
	case DataType::Float:				 return 1; break;
	case DataType::FloatVector2: return 2; break;
	case DataType::FloatVector3: return 3; break;
	case DataType::FloatVector4: return 4; break;
	default: break;
	}
	return 0;
}
