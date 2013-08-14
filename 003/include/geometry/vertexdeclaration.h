#if ! defined(__VERTEX_ATTRIBUTE__)
#define __VERTEX_ATTRIBUTE__

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <gl_loader/gl_loader.h>

struct VertexAttribute
{
	std::string name;
	GLenum type;
	size_t offset;
};

typedef std::vector<VertexAttribute> VertexAttributeList;

struct VertexDeclaration
{
	VertexDeclaration(size_t stride, const VertexAttribute* const attrs, size_t attrCount)
		: Stride(stride), Attributes(attrs, attrs + attrCount)
	{
	}

	VertexDeclaration(size_t stride, const VertexAttributeList& attrs)
		: Stride(stride), Attributes(attrs)
	{
	}

	const size_t Stride;
	const VertexAttributeList Attributes;
};


#endif // __VERTEX_ATTRIBUTE__
