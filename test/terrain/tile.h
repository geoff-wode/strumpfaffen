// A single (flat) tile in XZ plane...

#if ! defined(__TILE__)
#define __TILE__

#include <stddef.h>
#include "../device.h"
#include "../shader.h"

class TerrainTile
{
public:
	TerrainTile(int numCells);
	~TerrainTile();

	void Draw(Device& device, Shader shader);

private:
	size_t				numElements;
	RenderState		renderState;
};

#endif // __TILE__
