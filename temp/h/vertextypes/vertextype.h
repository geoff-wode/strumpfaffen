#if ! defined(__VERTEX_TYPE_H__)
#define __VERTEX_TYPE_H__

#include <vertextypes/vertexdeclaration.h>

namespace Framework
{
	class IVertexType
	{
	public:
		virtual const VertexDeclaration& GetVertexDeclaration() const = 0;
	};
}

#endif // __VERTEX_TYPE_H__
