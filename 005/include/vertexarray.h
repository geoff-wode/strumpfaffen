// A vertex array contains all the information needed to define a mesh, where a mesh
// is composed of an (optional) index buffer and one or more vertex buffers, each of
// which in turn contains a stream of records defining vertex attributes.

#if ! defined(__VERTEX_ARRAY__)
#define __VERTEX_ARRAY__

#include <string>
#include <vector>
#include <gl_loader/gl_loader.h>
#include <buffers.h>
#include <vertexdeclaration.h>

// Defines a complete "mesh", consisting of one or more buffers of vertex
// attribute data and an (optional) index buffer.
class VertexArray
{
public:
	typedef std::vector<VertexDeclarationPtr> DeclarationList;

	VertexArray(const DeclarationList& declarations);
	VertexArray(const DeclarationList& declarations, IndexBufferPtr indexBuffer);

	VertexArray(const VertexDeclarationPtr* declarations, size_t declCount);
	VertexArray(const VertexDeclarationPtr* declarations, size_t declCount, IndexBufferPtr indexBuffer);

	~VertexArray();

	void Enable();

	const IndexBufferPtr indexBuffer;
	const DeclarationList declarations;

private:
	GLuint vao;
};

#endif // __VERTEX_ARRAY__
