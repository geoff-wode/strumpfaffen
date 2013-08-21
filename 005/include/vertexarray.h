// A vertex array contains all the information needed to define a mesh, where a mesh
// is composed of an (optional) index buffer and one or more vertex buffers, each of
// which in turn contains a stream of records defining vertex attributes.

#if ! defined(__VERTEX_ARRAY__)
#define __VERTEX_ARRAY__

#include <string>
#include <vector>
#include <gl_loader/gl_loader.h>
#include <buffers.h>

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

// Defines the layout and location of a stream of vertex attributes.
struct VertexDeclaration
{
	typedef boost::shared_ptr<VertexBuffer> VertexBufferPtr;
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
	const VertexBufferPtr vertexBuffer;
	const VertexAttributeList attributes;
};
typedef boost::shared_ptr<VertexDeclaration> VertexDeclarationPtr;

// Defines a complete "mesh", consisting of one or more buffers of vertex
// attribute data and an (optional) index buffer.
class VertexArray
{
public:
	typedef std::vector<VertexDeclarationPtr> DeclarationList;

	VertexArray(const DeclarationList& declarations);
	VertexArray(const DeclarationList& declarations, boost::shared_ptr<IndexBuffer> indexBuffer);

	VertexArray(const VertexDeclarationPtr* declarations, size_t declCount);
	VertexArray(const VertexDeclarationPtr* declarations, size_t declCount, boost::shared_ptr<IndexBuffer> indexBuffer);

	~VertexArray();

	void Enable();

	const boost::shared_ptr<IndexBuffer> indexBuffer;
	const DeclarationList declarations;

private:
	GLuint vao;
};

#endif // __VERTEX_ARRAY__
