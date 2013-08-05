// Describes a transformable model mesh

#if ! defined(__MESH_NODE_H__)
#define __MESH_NODE_H__

#include <string>
#include <scenegraph/transformnode.h>
#include <geometry/mesh.h>

namespace SceneGraph
{
	class MeshNodeClass : public TransformNodeClass
	{
	public:
		MeshNodeClass(const std::string& filename);
		virtual ~MeshNodeClass();

		virtual void LoadResources(Scene* const scene);
		virtual bool PreRender(Scene* const scene);
		virtual void Render(Scene* const scene);

	private:
		std::string filename;
		Geometry::Mesh mesh;
	};

	typedef boost::shared_ptr<MeshNodeClass> MeshNode;
}

#endif // __MESH_NODE_H__
