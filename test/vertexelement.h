#if ! defined(__VERTEX_ELEMENT__)
#define __VERTEX_ELEMENT__

#include <string>

struct VertexElement
{
	std::string		semantic;
	size_t				offset;		// from the start of the vertex layout to this element
};

#endif // __VERTEX_ELEMENT__
