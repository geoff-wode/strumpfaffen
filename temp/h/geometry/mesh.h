// A mesh is a high-level container describing a set of vertices which all share the
// same set of attributes.
// Meshes are created by providing list(s) of values for each attribute type.
// A vertex array can then be created from the mesh data.

#if ! defined(__MESH_H__)
#define __MESH_H__

#include <string>
#include <boost/shared_ptr.hpp>
#include <buffers/indexbuffer.h>
#include <buffers/vertexbuffer.h>
#include <vertextypes/vertextype.h>

namespace Geometry
{

	class IMesh
	{
	public:
		virtual void Draw() const = 0;

	protected:
		~IMesh();
	};

	typedef boost::shared_ptr<IMesh>	Mesh;

	// Create a new mesh from a vertex buffer and (optional) index buffer.
	Mesh CreateMesh(Buffers::VertexBuffer vb);
	Mesh CreateMesh(Buffers::VertexBuffer vb, Buffers::IndexBuffer ib);

	// Create a mesh from a file on disk.
	Mesh CreateMesh(const std::string& filename);
}

#endif // __MESH_H__
