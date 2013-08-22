#if ! defined(__RENDER_VERTEXATTRIBUTE__)
#define __RENDER_VERTEXATTRIBUTE__

#include <stdlib.h>

// A structure that defines how a given vertex attribute is passed to a shader.
//
// offset				from the beginning of a single vertex to this attribute (ie. offsetof(VertexStruct, attribute) )
// elements			of the attribute (e.g. vec3 = 3, float = 1, etc.)
struct VertexAttribute
{
	size_t offset;
	size_t elements;
};


#endif // __RENDER_VERTEXATTRIBUTE__
