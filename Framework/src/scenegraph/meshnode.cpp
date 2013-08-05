#include <scenegraph/meshnode.h>
#include <scenegraph/scene.h>

using namespace SceneGraph;

//---------------------------------------------------------------------------

MeshNodeClass::MeshNodeClass(const std::string& filename)
	: filename(filename)
{
}

MeshNodeClass::~MeshNodeClass()
{
}

void MeshNodeClass::LoadResources(Scene* const scene)
{
	mesh = Geometry::LoadMesh(filename);
}

bool MeshNodeClass::PreRender(Scene* const scene)
{
	return mesh;
}

void MeshNodeClass::Render(Scene* const scene)
{
	mesh->Render();
}
