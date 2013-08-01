#include <geometry/mesh.h>
#include <gl_loader/gl_loader.h>
#include <vertexarray/vertexarray.h>
#include <buffers/vertexbuffer.h>
#include <core/debug.h>
#include <glm/glm.hpp>
#include <vertextypes/vertexposition.h>
#include <vertextypes/vertexpositionnormal.h>
#include <vertextypes/vertexpositiontexture.h>
#include <vertextypes/vertexpositionnormaltexture.h>

using namespace Geometry;
using namespace Framework;

//-------------------------------------------------------------------------------

struct MeshDescription
{
	std::vector<glm::vec3> positionsList;
	std::vector<glm::vec3> normalsList;
	std::vector<glm::vec2> texCoordsList;

	std::vector<unsigned int> positionIndices;
	std::vector<unsigned int> normalIndices;
	std::vector<unsigned int> texCoordIndices;
};

//-------------------------------------------------------------------------------

static bool LoadOBJFile(const std::string& filename, MeshDescription& description);
static Buffers::VertexBuffer BuildVertexBuffer(const MeshDescription& description);

//-------------------------------------------------------------------------------

IMesh::~IMesh()
{
}

//-------------------------------------------------------------------------------

class MeshImpl : public IMesh
{
public:
	MeshImpl(Buffers::VertexBuffer vb, Buffers::IndexBuffer ib)
		: va(VertexArrays::CreateVertexArray(vb, ib))
	{
	}

	virtual ~MeshImpl()
	{
	}

	virtual void Draw() const
	{
		va->Activate();
		if (va->GetIndexBuffer())
		{
			GLenum elementType;
			switch (va->GetIndexBuffer()->ElementType())
			{
			case Buffers::IndexBufferType::Byte: elementType = GL_UNSIGNED_BYTE; break;
			case Buffers::IndexBufferType::Short: elementType = GL_UNSIGNED_SHORT; break;
			case Buffers::IndexBufferType::Int: elementType = GL_UNSIGNED_INT; break;
			default: ASSERT(false);
			}
			glDrawElements(GL_TRIANGLES, va->GetIndexBuffer()->ElementCount(), elementType, 0);
		}
		else
		{
			glDrawArrays(GL_TRIANGLES, 0, va->GetVertexBuffer()->VertexCount());
		}
		va->Deactivate();
	}
	
	VertexArrays::VertexArray va;
};

//-------------------------------------------------------------------------------

Mesh Geometry::CreateMesh(Buffers::VertexBuffer vb)
{
	Mesh m(new MeshImpl(vb, NULL));
	return m;
}

Mesh Geometry::CreateMesh(Buffers::VertexBuffer vb, Buffers::IndexBuffer ib)
{
	Mesh m(new MeshImpl(vb, ib));
	return m;
}

Mesh Geometry::CreateMesh(const std::string& filename)
{
	MeshDescription description;

	if (!LoadOBJFile(filename, description))
	{
		return NULL;
	}

	// An OBJ file doesn't allow the (easy!) creation of an index buffer, so don't bother...
	Buffers::VertexBuffer vb = BuildVertexBuffer(description);
	Mesh m(new MeshImpl(vb, NULL));

	return m;
}

//--------------------------------------------------------

static Buffers::VertexBuffer BuildVertexBuffer(const MeshDescription& description)
{
	const bool positionsDefined = !description.positionsList.empty();
	const bool normalsDefined = !description.normalsList.empty();
	const bool texCoordsDefined = !description.texCoordsList.empty();

	// Based on which fields are present, compute the size of one vertex and create a suitable vertex declaration...
	std::vector<Framework::VertexElement> vertexElements;
	size_t vertexStride = 0;
	size_t offset = 0;

	if (positionsDefined)
	{
		static const Framework::VertexElement e = { "Position", Framework::VertexElement::DataType::FloatVector3, offset };
		vertexElements.push_back(e);
		vertexStride += sizeof(glm::vec3);
		offset += sizeof(glm::vec3);
	}
	if (normalsDefined)
	{
		static const Framework::VertexElement e = { "Normal", Framework::VertexElement::DataType::FloatVector3, offset };
		vertexElements.push_back(e);
		vertexStride += sizeof(glm::vec3);
		offset += sizeof(glm::vec3);
	}
	if (texCoordsDefined)
	{
		static const Framework::VertexElement e = { "TexCoord", Framework::VertexElement::DataType::FloatVector2, offset };
		vertexElements.push_back(e);
		vertexStride += sizeof(glm::vec2);
	}

	const Framework::VertexDeclaration vertexDeclaration = Framework::VertexDeclaration(vertexElements.size(), vertexElements.data());

	// Now populate a data buffer with the combined (i.e. de-indexed) vertex data.
	// This is because OBJ files do not lend themselves to indexed buffers, since there are up to 3 data streams,
	// each with its own index and we can only have one index buffer per mesh. Not super-efficient but...
	
	const size_t vertexCount = description.positionIndices.size();
	std::vector<float> vertexData(vertexCount * vertexStride);
	offset = 0;

	for (size_t i = 0; i < vertexCount; ++i)
	{
		if (positionsDefined)
		{
			const size_t index = description.positionIndices[i];
			std::memcpy(vertexData.data() + offset, &description.positionsList[index - 1], sizeof(glm::vec3));
			offset += sizeof(glm::vec3);
		}

		if (normalsDefined)
		{
			const size_t index = description.normalIndices[i];
			std::memcpy(vertexData.data() + offset, &description.normalsList[index - 1], sizeof(glm::vec3));
			offset += sizeof(glm::vec3);
		}

		if (texCoordsDefined)
		{
			const size_t index = description.texCoordIndices[i];
			std::memcpy(vertexData.data() + offset, &description.texCoordsList[index - 1], sizeof(glm::vec2));
			offset += sizeof(glm::vec2);
		}
	}

	// Now create & populate the buffer...
	Buffers::VertexBuffer vb = Buffers::CreateVertexBuffer(vertexDeclaration, vertexCount, Buffers::UsageHint::Static);
	vb->SetData(vertexData.data(), vertexCount, 0);

	return vb;
}

//--------------------------------------------------------

static bool LoadOBJFile(const std::string& filename, MeshDescription& description)
{
	FILE* in = fopen(filename.c_str(), "rb");

	if (NULL == in)
	{
		return false;
	}

	char lineHeader[128];

	for (int result = fscanf(in, "%s", lineHeader); EOF != result; result = fscanf(in, "%s", lineHeader))
	{
		if (0 == strcmp("v", lineHeader))
		{
			float x, y, z;
			fscanf(in, "%f %f %f\n", &x, &y, &z);
			description.positionsList.push_back(glm::vec3(x, y, z));
		}
		else if (0 == strcmp("vt", lineHeader))
		{
			float u, v;
			fscanf(in, "%f %f\n", &u, &v);
			description.texCoordsList.push_back(glm::vec2(u, 1.0f - v)); // need to invert v to use DDS-DXT textures in GL
		}
		else if (0 == strcmp("vn", lineHeader))
		{
			float x, y, z;
			fscanf(in, "%f %f %f\n", &x, &y, &z);
			description.normalsList.push_back(glm::vec3(x, y, z));
		}
		else if (0 == strcmp("f", lineHeader))
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
				description.positionIndices.push_back(vertexIndex[i]);
				description.normalIndices.push_back(normalIndex[i]);
				description.texCoordIndices.push_back(uvIndex[i]);
			}
		}
	}

	fclose(in);

	return true;
}

static bool LoadFile(const std::string& filename, std::string& content)
{
	errno = 0;
	FILE* in = fopen(filename.c_str(), "rb");
	if (NULL != in)
	{
		fseek(in, 0, SEEK_END);
		content.resize(ftell(in));
		rewind(in);
		fread(&content[0], sizeof(char), content.size(), in);
		fclose(in);
	}
	else
	{
		LOG("%s - errno: %s\n", filename.c_str(), strerror(errno));
	}
	return (NULL != in);
}
