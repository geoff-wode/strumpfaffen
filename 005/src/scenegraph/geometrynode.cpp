#include <scenegraph/scene.h>
#include <scenegraph/geometrynode.h>

//------------------------------------------------------------

GeometryNode::GeometryNode()
	: TransformNode(glm::mat4(1))
{
}

GeometryNode::~GeometryNode()
{
}

//------------------------------------------------------------
void GeometryNode::LoadNodeContent(Scene* const scene)
{
}

//------------------------------------------------------------
void GeometryNode::RenderNode(Scene* const scene)
{
}
