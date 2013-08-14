#if ! defined(__SCENEGRAPH_MESHNODE_H__)
#define __SCENEGRAPH_MESHNODE_H__

#include <framework/scenegraph/transformnode.h>
#include <framework/drawstate.h>

namespace Framework
{
	class MeshNode : public TransformNode
	{
	public:
		MeshNode();
		virtual ~MeshNode();
		virtual void LoadResources(Scene* const scene);
		virtual void Render(Scene* const scene);

	private:
		Framework::DrawState drawState;
	};
}

#endif // __SCENEGRAPH_MESHNODE_H__
