#include <gl_loader/gl_loader.h>
#include <geometry/mesh.h>
#include <renderstate.h>

//-----------------------------------------------------------

Mesh::Mesh()
	: vertexArrays(new std::vector<VertexArray>())
{
}

Mesh::~Mesh()
{
}
