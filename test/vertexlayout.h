#if ! defined(__VERTEX_LAYOUT__)
#define __VERTEX_LAYOUT__

#include "vertexelement.h"

struct VertexLayout
{
	const size_t				stride;
	const size_t				elementCount;
	static const size_t MaxElements = 16;
	const VertexElement elements[MaxElements];
};


#endif // __VERTEX_LAYOUT__
