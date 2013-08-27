#if ! defined(__TERRAIN_TILE_NODE__)
#define __TERRAIN_TILE_NODE__

#include <renderstate.h>
#include <scenegraph/transformnode.h>

class TileNode : public TransformNode
{
public:
	// Create a tile on the XZ plane which is gridSize points in width and length.
	// For texturing to work correctly, gridSize should be (2^N)+1 in size.
	TileNode(SceneNodePtr parent, size_t gridSize);

	virtual void LoadContent(Scene* const scene);
	virtual bool PreRender(Scene* const scene);
	virtual void Render(Scene* const scene);

protected:
	const size_t	gridSize;
	const size_t	numElements;
	VertexBuffer	vertexBuffer;
	IndexBuffer		indexBuffer;
};

#endif // __TERRAIN_TILE_NODE__
