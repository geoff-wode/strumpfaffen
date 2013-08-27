#if ! defined(__VERTEX_ATTRIBUTE__)
#define __VERTEX_ATTRIBUTE__

#include <string>
#include <gl_loader/gl_loader.h>

// Defines a single attribute in a structure of vertex attributes.
// Attributes are composed of elements, fundamental data types (like float).
// By associating the correct "semantic" with the attribute, attributes can be spread
// across multiple buffers without having to worry about keeping their shader location
// values in sync.

struct VertexAttribute
{
	VertexAttribute(const std::string& semantic, GLenum type, size_t size, size_t offset)
		: semantic(semantic), type(type), size(size), offset(offset)
	{
	}

	std::string semantic;
	GLenum type;		// type of each element
	size_t size;		// number of elements
	size_t offset;	// from start of structure (ie. "offsetof(Struct, Member)")
};

#endif // __VERTEX_ATTRIBUTE__
