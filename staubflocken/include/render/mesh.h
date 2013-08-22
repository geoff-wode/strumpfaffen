#if ! defined(__MESH__)
#define __MESH__

#include <stddef.h>
#include <render/vertexattribute.h>
#include <buffers/vertexbuffer.h>
#include <boost/shared_ptr.hpp>
#include <gl_loader/gl_loader.h>

// A mesh is a set of interleaved vertex attributes stored in a single buffer.
// Eg. a simple textured object may have:
//
//			vec3 : position
//			vec2 : textureCoord
//
// so the mesh will contain 2 attributes.
class Mesh
{
public:
	Mesh(size_t attrCount, const VertexAttribute* attrs, VertexBufferPtr vertexBuffer);

	~Mesh();

	void Enable() const;

private:
	GLuint vao;
};

typedef boost::shared_ptr<Mesh> MeshPtr;

#endif // __MESH__
