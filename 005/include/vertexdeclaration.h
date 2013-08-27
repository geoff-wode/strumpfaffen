#if ! defined(__VERTEX_DECLARATION__)
#define __VERTEX_DECLARATION__

#include <buffers.h>
#include <vertexattribute.h>
#include <boost/shared_ptr.hpp>

// Defines the layout and location of a stream of vertex attributes.
struct VertexDeclaration
{
	typedef std::vector<VertexAttribute> VertexAttributeList;
	
	VertexDeclaration(size_t stride, VertexBufferPtr vertexBuffer, const VertexAttributeList& attrs)
		: stride(stride), vertexBuffer(vertexBuffer), attributes(attrs)
	{
	}

	VertexDeclaration(size_t stride, VertexBufferPtr vertexBuffer, const VertexAttribute* const attrs, size_t attrCount)
		: stride(stride), vertexBuffer(vertexBuffer), attributes(attrs, attrs + attrCount)
	{
	}

	const size_t stride;
	VertexBufferPtr vertexBuffer;
	const VertexAttributeList attributes;
};
typedef boost::shared_ptr<VertexDeclaration> VertexDeclarationPtr;

#endif // __VERTEX_DECLARATION__
