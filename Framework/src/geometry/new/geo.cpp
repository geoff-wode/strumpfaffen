#include <vector>
#include <string>
#include <memory>
#include <boost/shared_ptr.hpp>
#include <glm/glm.hpp>
#include <gl_loader/gl_loader.h>
#include <util/debug.h>
#include <util/files.h>

#include <geometry/mesh.h>
#include <buffers/vertexbuffer.h>
#include <geometry/vertexdeclaration.h>

using namespace Geometry;

//----------------------------------------------------------------------------------

struct MeshVertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

static VertexComponent meshVertexComponents[] =
{
	{ GL_FLOAT, 3, offsetof(MeshVertex, position) },
	{ GL_FLOAT, 3, offsetof(MeshVertex, normal) },
	{ GL_FLOAT, 2, offsetof(MeshVertex, texCoord) }
};

static VertexDeclaration meshVertexDecl =
{
	sizeof(MeshVertex),
	sizeof(meshVertexComponents) / sizeof(meshVertexComponents[0]),
	meshVertexComponents
};

//-------------------------------------------------------------------

// A file is converted into a mesh description which is then used to
// expand the vertices into an un-indexed triangle soup.
struct MeshDescription
{
	std::vector<glm::vec3> positionsList;
	std::vector<glm::vec3> normalsList;
	std::vector<glm::vec2> texCoordsList;

	std::vector<unsigned int> positionIndices;
	std::vector<unsigned int> normalIndices;
	std::vector<unsigned int> texCoordIndices;
};

//----------------------------------------------------------------------------------

static void LoadMeshFile(const std::string& name, MeshDescription* const description);
static void BuildMesh(const MeshDescription* const description, class MeshImpl* const mesh);

//----------------------------------------------------------------------------------

class MeshImpl : public IMesh
{
public:
	virtual void Render()
	{
		vb->Enable();
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (const void*)offsetof(MeshVertex, position));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (const void*)offsetof(MeshVertex, normal));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex), (const void*)offsetof(MeshVertex, texCoord));

		glDrawArrays(GL_TRIANGLES, 0, primitiveCount);

		vb->Disable();
	}

	size_t primitiveCount;
	Buffers::VertexBuffer vb;
};

//----------------------------------------------------------------------------------

Mesh Geometry::LoadMesh(std::string& name)
{
	boost::shared_ptr<MeshImpl> mesh(new MeshImpl());
	
	MeshDescription description;

	LoadMeshFile(name, &description);
	BuildMesh(&description, mesh.get());

	return mesh;
}

//----------------------------------------------------------------------------------

static void LoadMeshFile(const std::string& name, MeshDescription* const description)
{
	FILE* in = fopen(name.c_str(), "rb");

	char line[128];

	for (int result = fscanf(in, "%s", line); EOF != result; result = fscanf(in, "%s", line))
	{
		if (0 == strcmp("v", line))
		{
			float x, y, z;
			fscanf(in, "%f %f %f\n", &x, &y, &z);
			description->positionsList.push_back(glm::vec3(x, y, z));
		}
		else if (0 == strcmp("vt", line))
		{
			float u, v;
			fscanf(in, "%f %f\n", &u, &v);
			description->texCoordsList.push_back(glm::vec2(u, 1.0f - v)); // need to invert v to use DDS-DXT textures in GL
		}
		else if (0 == strcmp("vn", line))
		{
			float x, y, z;
			fscanf(in, "%f %f %f\n", &x, &y, &z);
			description->normalsList.push_back(glm::vec3(x, y, z));
		}
		else if (0 == strcmp("f", line))
		{
			unsigned int vertexIndex[3];
			unsigned int normalIndex[3];
			unsigned int uvIndex[3];

			const int matches = fscanf(in, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&vertexIndex[0], &uvIndex[0], &normalIndex[0],
				&vertexIndex[1], &uvIndex[1], &normalIndex[1],
				&vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			ASSERTM(9 == matches, "unexpected OBJ format (faces must be in 'v/t/n v/t/n v/t/n' format)\n");

			for (int i = 0; i < 3; ++i)
			{
				description->positionIndices.push_back(vertexIndex[i]);
				description->normalIndices.push_back(normalIndex[i]);
				description->texCoordIndices.push_back(uvIndex[i]);
			}
		}
	}

	fclose(in);
}

//----------------------------------------------------------------------------------

static void BuildMesh(const MeshDescription* const description, MeshImpl* mesh)
{
	// populate a buffer with the de-indexed vertices...
	const size_t vertexCount = description->positionIndices.size();
	std::vector<MeshVertex> vertexData(vertexCount);
	for (size_t i = 0; i < vertexCount; ++i)
	{
		const size_t posIndex = description->positionIndices[i] - 1;
		const size_t normIndex = description->normalIndices[i] - 1;
		const size_t texIndex = description->texCoordIndices[i] - 1;

		vertexData[i].position = description->positionsList[posIndex];
		vertexData[i].normal = description->normalsList[normIndex];
		vertexData[i].texCoord = description->texCoordsList[texIndex];
	}

	// create the vertex buffer...
	const size_t bufferSize = sizeof(MeshVertex) * vertexCount;
	mesh->vb = Buffers::CreateVertexBuffer(bufferSize, Buffers::UsageHint::Static);
	mesh->vb->SetData(vertexData.data(), bufferSize, 0);

	mesh->primitiveCount = vertexCount / 3;
}
