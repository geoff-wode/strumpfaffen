#if ! defined(__GRID__)
#define __GRID__

#include <scenegraph/transformnode.h>
#include <geometry/vertexarray.h>
#include <renderstate.h>

class Grid : public TransformNode
{
public:
	Grid(size_t size, const glm::mat4& transform);
	virtual ~Grid();

	virtual void Load(Scene* const scene);
	virtual void Render(Scene* const scene);

protected:
	const size_t size;
	size_t numTriangles;
	boost::shared_ptr<VertexArray> vertexArray;
	RenderState renderState;
};

#endif // __GRID__
