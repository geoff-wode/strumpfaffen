// A single (flat) tile in XZ plane...

#include <terrain/tile.h>
#include <device.h>
#include <vector>
#include <glm/glm.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <gl_loader/gl_loader.h>

//--------------------------------------------------------------------------------------------

struct Vertex
{
	glm::vec3 position;
	glm::vec2 textureCoord;
};

//--------------------------------------------------------------------------------------------
static const VertexLayout	vertexLayout =
{
	sizeof(Vertex),
	2,
	{
		{ "Position", offsetof(Vertex, position) },
		{ "Texture0", offsetof(Vertex, textureCoord) }
	}
};

//--------------------------------------------------------------------------------------------

// Create the tile's vertex buffer.
// N x N vertices are created on the XZ plane and centred on (0,0,0).
// Texture coordinates range from (0,0) at (0,0,0) to (1,1) at (N,0,N).
static void BuildVertices(int size, std::vector<Vertex>& vertices);

// Create the tile's index buffer.
static void BuildIndices(int size, std::vector<unsigned short>& indices);

//--------------------------------------------------------------------------------------------

TerrainTile::TerrainTile(int numCells)
{
	const int size = numCells + 1;

	std::vector<Vertex> vertices(size * size);
	BuildVertices(size, vertices);
	renderState.vertexBuffer = boost::make_shared<VertexBufferClass>(vertices.size() * sizeof(Vertex), GL_STATIC_DRAW);
	renderState.vertexBuffer->Enable();
	renderState.vertexBuffer->SetData(vertices.data(), vertices.size() * sizeof(Vertex), 0);
	renderState.vertexBuffer->Disable();

	numElements = 2 * size * (size - 1);
	std::vector<unsigned short> indices(numElements);
	BuildIndices(size, indices);

	renderState.indexBuffer = boost::make_shared<IndexBufferClass>(numElements * sizeof(unsigned short), GL_STATIC_DRAW, GL_UNSIGNED_SHORT);
	renderState.indexBuffer->Enable();
	renderState.indexBuffer->SetData(indices.data(), numElements * sizeof(unsigned short), 0);
	renderState.indexBuffer->Disable();

	renderState.vertexLayout = &vertexLayout;
	renderState.polygonMode = GL_LINE;
}

//--------------------------------------------------------------------------------------------
TerrainTile::~TerrainTile()
{
}

//--------------------------------------------------------------------------------------------
void TerrainTile::Draw(Device& device, Shader shader)
{
	renderState.shader = shader;
	device.Draw(GL_TRIANGLE_STRIP, 0, numElements, renderState);
}

//--------------------------------------------------------------------------------------------

static void BuildVertices(int size, std::vector<Vertex>& vertices)
{
	const float step(1.0f / (float)size);
	int index = 0;
	for (int z = 0; z < size; ++z)
	{
		for (int x = 0; x < size; ++x)
		{
			Vertex v;
			v.position = glm::vec3((float)x - (0.5f * (size-1)), 0, -z + (0.5f * (size-1)));
			v.textureCoord = glm::vec2(step * x, step * z);
			vertices[index++] = v;
		}
	}
}

//--------------------------------------------------------------------------------------------
static void BuildIndices(int size, std::vector<unsigned short>& indices)
{
	int index = 0;
	int z = 0;
	while (z < size - 1)
	{
		for (int x = 0; x < size; x++)
		{
			indices[index++] = x + (z * size);
			indices[index++] = x + ((z + 1) * size);
		}
		++z;
		if (z < size - 1)
		{
			for (int x = size - 1; x >= 0; x--)
			{
				indices[index++] = x + ((z + 1) * size);
				indices[index++] = x + (z * size);
			}
		}
		++z;
	}
}
