#include <scenegraph/scene.h>
#include <scenegraph/grid.h>

//---------------------------------------------------------------------

struct Vertex
{
	glm::vec3 position;
	glm::vec2 textureCoord;

	static const VertexDeclaration* GetVertexDecl()
	{
		static const VertexAttribute attrs[] =
		{
			{ "VertexPosition", GL_FLOAT_VEC3, offsetof(Vertex, position) },
			{ "VertexTexCoord0", GL_FLOAT_VEC2, offsetof(Vertex, textureCoord) }
		};
		static const size_t attrCount = sizeof(attrs) / sizeof(attrs[0]);
		static VertexDeclaration vertexDecl(sizeof(Vertex), attrs, attrCount);

		return &vertexDecl;
	}
};

//---------------------------------------------------------------------

static void DefineVertices(int size, boost::shared_ptr<VertexArray> vertexArray);
static void DefineIndices(int size, boost::shared_ptr<VertexArray> vertexArray);

//---------------------------------------------------------------------

Grid::Grid(size_t size, const glm::mat4& transform)
	: TransformNode(transform), size(size)
{
}

//---------------------------------------------------------------------
Grid::~Grid()
{
}

//---------------------------------------------------------------------
void Grid::Load(Scene* const scene)
{
	// Build the vertex array for the grid and set up the render state...
	vertexArray = boost::make_shared<VertexArray>();
	vertexArray->vertexDecl = Vertex::GetVertexDecl();
	DefineVertices(size, vertexArray);
	DefineIndices(size, vertexArray);
	renderState.vertexArray = vertexArray;
}

//---------------------------------------------------------------------
void Grid::Render(Scene* const scene)
{
}

//---------------------------------------------------------------------

static void DefineVertices(int size, boost::shared_ptr<VertexArray> vertexArray)
{
	std::vector<Vertex> vertices(size * size);
	int i = 0;
	for (int z = 0; z < size; z++)
	{
		for (int x = 0; x < size; ++x)
		{
			vertices[i].position = glm::vec3(x, 0, -z);
			vertices[i].textureCoord = glm::vec2((float)x / (float)size, (float)z / (float)size);
			++i;
		}
	}

	const size_t vbSize = sizeof(vertices[0]) * vertices.size();
	vertexArray->vertexBuffer = boost::make_shared<Buffer>(vbSize, GL_ARRAY_BUFFER, GL_STATIC_DRAW);
	vertexArray->vertexBuffer->Bind();
	vertexArray->vertexBuffer->SetData(vertices.data(), vbSize, 0);
	vertexArray->vertexBuffer->Unbind();
}

//---------------------------------------------------------------------
static void DefineIndices(int size, boost::shared_ptr<VertexArray> vertexArray)
{
	std::vector<unsigned short> indices(2 * size * (size -1));

	int i = 0;
	int z = 0;
	while (z < size - 1)
	{
		for (int x = 0; x < size; ++x)
		{
			indices[i++] = x + (z * size);
			indices[i++] = x + ((z + 1) * size);
		}
		++z;

		if (z < size - 1)
		{
			for (int x = size - 1; x >= 0; --x)
			{
				indices[i++] = x + ((z + 1) * size);
				indices[i++] = x + (z * size);
			}
		}
		++z;
	}

	const size_t ibSize = indices.size() * sizeof(indices[0]);
	vertexArray->indexBuffer = boost::make_shared<Buffer>(ibSize, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
	vertexArray->indexBuffer->Bind();
	vertexArray->indexBuffer->SetData(indices.data(), ibSize, 0);
	vertexArray->indexBuffer->Unbind();
}
