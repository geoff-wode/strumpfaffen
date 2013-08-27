#if ! defined(__GEOMETRY_NODE__)
#define __GEOMETRY_NODE__

#include <scenegraph/transformnode.h>

class GeometryNode : public TransformNode
{
public:
	GeometryNode();
	virtual ~GeometryNode();

protected:
	virtual void LoadNodeContent(Scene* const scene);
	virtual void RenderNode(Scene* const scene);
};

#endif // __GEOMETRY_NODE__
