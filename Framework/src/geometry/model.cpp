#include <util/debug.h>
#include <gl_loader/gl_loader.h>
#include <geometry/model.h>
#include <glm/glm.hpp>
#include <ctype.h>
#include <vector>
#include <buffers/vertexbuffer.h>
#include <core/vertexdeclaration.h>
#include <core/vertexpositionnormaltexture.h>

using namespace Geometry;

//-------------------------------------------------------------------

class ModelImpl : public IModel
{
public:
	ModelImpl(Buffers::VertexBuffer vb)
		: vb(vb)
	{
	}

	virtual ~ModelImpl()
	{
	}

	virtual void Render()
	{
		if (vb)
		{
			vb->Activate();
			glDrawArrays(GL_TRIANGLES, 0, vb->VertexCount());
			vb->Deactivate();
		}
	}

	Buffers::VertexBuffer vb;
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

//-------------------------------------------------------------------

static void LoadOBJFile(const std::string& filename, MeshDescription& mesh);
static Buffers::VertexBuffer BuildVertexBuffer(const MeshDescription& mesh);

//-------------------------------------------------------------------

Model Geometry::LoadModel(const std::string& filename)
{
	Buffers::VertexBuffer vb;

	// find the right-most dot in the filename to get the start of the extension...
	const size_t pos = filename.rfind(".");
	// then, as long as it was found, look for a recognised extension...
	if (std::string::npos != pos)
	{
		std::string extension(filename, pos + 1);
		for (size_t i = 0; i < extension.size(); ++i) { extension[i] = tolower(extension[i]); }

		if (0 == strcmp("obj", extension.c_str()))
		{
			MeshDescription mesh;
			LoadOBJFile(filename, mesh);
			vb = BuildVertexBuffer(mesh);
		}
		else
		{
			LOG("unrecognised file type %s\n", extension.c_str());
		}
	}

	if (vb)
	{
		Model model(new ModelImpl(vb));
		return model;
	}

	return NULL;
}

//-------------------------------------------------------------------
static void LoadOBJFile(const std::string& filename, MeshDescription& mesh)
{
	FILE* in = fopen(filename.c_str(), "rb");

	char line[128];

	for (int result = fscanf(in, "%s", line); EOF != result; result = fscanf(in, "%s", line))
	{
		if (0 == strcmp("v", line))
		{
			float x, y, z;
			fscanf(in, "%f %f %f\n", &x, &y, &z);
			mesh.positionsList.push_back(glm::vec3(x, y, z));
		}
		else if (0 == strcmp("vt", line))
		{
			float u, v;
			fscanf(in, "%f %f\n", &u, &v);
			mesh.texCoordsList.push_back(glm::vec2(u, 1.0f - v)); // need to invert v to use DDS-DXT textures in GL
		}
		else if (0 == strcmp("vn", line))
		{
			float x, y, z;
			fscanf(in, "%f %f %f\n", &x, &y, &z);
			mesh.normalsList.push_back(glm::vec3(x, y, z));
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
				mesh.positionIndices.push_back(vertexIndex[i]);
				mesh.normalIndices.push_back(normalIndex[i]);
				mesh.texCoordIndices.push_back(uvIndex[i]);
			}
		}
	}

	fclose(in);
}

//-------------------------------------------------------------------

static Buffers::VertexBuffer BuildVertexBuffer(const MeshDescription& mesh)
{
	// populate a buffer with the de-indexed vertices...
	const size_t vertexCount = mesh.positionIndices.size();
	std::vector<Core::VertexPositionNormalTexture> vertexData(vertexCount);
	for (size_t i = 0; i < vertexCount; ++i)
	{
		const size_t posIndex = mesh.positionIndices[i] - 1;
		const size_t normIndex = mesh.normalIndices[i] - 1;
		const size_t texIndex = mesh.texCoordIndices[i] - 1;

		vertexData[i].Position = mesh.positionsList[posIndex];
		vertexData[i].Normal = mesh.normalsList[normIndex];
		vertexData[i].TexCoord = mesh.texCoordsList[texIndex];
	}

	// create a vertex declaration that reflects the data stored in the buffer.
	// (note that this data must be static and constant).
	static const Core::VertexElement elements[] =
	{
		Core::VertexElement("Position", Core::VertexElementType::FloatVector3, offsetof(Core::VertexPositionNormalTexture, Position)),
		Core::VertexElement("Normal", Core::VertexElementType::FloatVector3, offsetof(Core::VertexPositionNormalTexture, Normal)),
		Core::VertexElement("TexCoord", Core::VertexElementType::FloatVector2, offsetof(Core::VertexPositionNormalTexture, TexCoord))
	};
	static const Core::VertexDeclaration vDecl(3, elements);

	// create the vertex buffer...
	Buffers::VertexBuffer vb = Buffers::CreateVertexBuffer(vDecl, vertexCount, Buffers::UsageHint::Static);
	if (vb)
	{
		vb->SetData(vertexData.data(), vertexCount, 0);
	}
	return vb;
}
