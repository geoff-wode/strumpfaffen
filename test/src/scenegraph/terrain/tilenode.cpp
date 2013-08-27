#include <vector>
#include <scenegraph/scene.h>
#include <scenegraph/terrain/tilenode.h>

//--------------------------------------------------------------------------------------------

struct Vertex
{
	glm::vec3 position;
	glm::vec2 textureCoord;
};

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
template <typename T> static void BuildIndices(int size, std::vector<T>& indices);

//--------------------------------------------------------------------------------------------

TileNode::TileNode(size_t gridSize)
	: gridSize(gridSize),
		numElements(2 * gridSize * (gridSize - 1))
{
}

//--------------------------------------------------------------------------------------------

void TileNode::LoadContent(Scene* const scene)
{
	std::vector<Vertex> vertices(gridSize * gridSize);
	BuildVertices(gridSize, vertices);
	vertexBuffer = boost::make_shared<VertexBufferClass>(vertices.size() * sizeof(Vertex), GL_STATIC_DRAW);
	vertexBuffer->SetData(vertices.data(), vertices.size() * sizeof(Vertex), 0);

	// Use the fewest possible bits for index values...
	if (numElements <= UCHAR_MAX)
	{
		std::vector<GLubyte> indices(numElements);
		BuildIndices(gridSize, indices);
		indexBuffer = boost::make_shared<IndexBufferClass>(numElements * sizeof(GLubyte), GL_STATIC_DRAW, GL_UNSIGNED_BYTE);
		indexBuffer->SetData(indices.data(), numElements * sizeof(GLubyte), 0);
	}
	else if (numElements <= USHRT_MAX)
	{
		std::vector<GLushort> indices(numElements);
		BuildIndices(gridSize, indices);
		indexBuffer = boost::make_shared<IndexBufferClass>(numElements * sizeof(GLushort), GL_STATIC_DRAW, GL_UNSIGNED_SHORT);
		indexBuffer->SetData(indices.data(), numElements * sizeof(GLushort), 0);
	}
	else
	{
		std::vector<GLuint> indices(numElements);
		BuildIndices(gridSize, indices);
		indexBuffer = boost::make_shared<IndexBufferClass>(numElements * sizeof(GLuint), GL_STATIC_DRAW, GL_UNSIGNED_INT);
		indexBuffer->SetData(indices.data(), numElements * sizeof(GLuint), 0);
	}
}

//--------------------------------------------------------------------------------------------
bool TileNode::PreRender(Scene* const scene)
{
	// TODO: perform visibility test

	return true;
}

//--------------------------------------------------------------------------------------------
void TileNode::Render(Scene* const scene)
{
	scene->renderState.indexBuffer = indexBuffer;
	scene->renderState.vertexBuffer = vertexBuffer;
	scene->renderState.vertexLayout = &vertexLayout;

	scene->device.Draw(GL_TRIANGLE_STRIP, 0, numElements, scene->renderState);
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
template <typename T>
static void BuildIndices(int size, std::vector<T>& indices)
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
