#include <model.h>
#include <debug.h>
#include <vertexarray.h>
#include <vertextypes.h>
#include <stdio.h>
#include <ctype.h>
#include <vector>
#include <memory.h>
#include <gl_loader/gl_loader.h>
#include <glm/glm.hpp>
#include <Shlwapi.h>
#include <boost/scoped_ptr.hpp>

using namespace Terrene;

//----------------------------------------------------------------------------------

class ModelImpl : public IModel
{
public:
	ModelImpl() { }

	virtual void Render()
	{
		if (vertexArray)
		{
			vertexArray->Activate();
			glDrawArrays(GL_TRIANGLES, 0, vertexCount);
		}
	}

	unsigned int vertexCount;
	VertexArray	vertexArray;
};

//----------------------------------------------------------------------------------

static bool MatchString(const char* const a, const char* const b, bool caseSensitive);
static bool LoadOBJModel(const std::string& path, Model model);

//----------------------------------------------------------------------------------

Model Terrene::LoadModel(const std::string& filename)
{
	Model model(new ModelImpl());

	if (model)
	{
		bool loaded = false;

		const char* const suffix = PathFindExtension(filename.c_str());

		if (0 == strcmp(".obj", suffix))
		{
			loaded = LoadOBJModel("models\\" + filename, model);
		}

		if (!loaded)
		{
			model.reset();
		}
	}

	return model;
}

//----------------------------------------------------------------------------------

static void ReadOBJFileData(
	FILE* file, 
	std::vector<unsigned int>& vertexIndices,
	std::vector<unsigned int>& normalIndices,
	std::vector<unsigned int>& uvIndices,
	std::vector<glm::vec3>& vertices,
	std::vector<glm::vec3>& normals,
	std::vector<glm::vec2>& texCoords)
{
	char line[256];

	for (int result = fscanf(file, "%s", line); EOF != result; result = fscanf(file, "%s", line))
	{
		if (0 == strcmp("v", line))
		{
			float x, y, z;
			fscanf(file, "%f %f %f\n", &x, &y, &z);
			vertices.push_back(glm::vec3(x, y, z));
		}
		else if (0 == strcmp("vt", line))
		{
			float u, v;
			fscanf(file, "%f %f\n", &u, &v);
			texCoords.push_back(glm::vec2(u, 1.0f - v)); // need to invert v to use DDS-DXT textures file GL
		}
		else if (0 == strcmp("vn", line))
		{
			float x, y, z;
			fscanf(file, "%f %f %f\n", &x, &y, &z);
			normals.push_back(glm::vec3(x, y, z));
		}
		else if (0 == strcmp("f", line))
		{
			unsigned int vertexIndex[3];
			unsigned int normalIndex[3];
			unsigned int uvIndex[3];

			const int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
				&vertexIndex[0], &uvIndex[0], &normalIndex[0],
				&vertexIndex[1], &uvIndex[1], &normalIndex[1],
				&vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			ASSERTM(9 == matches, "unexpected OBJ format (faces must be file 'v/t/n v/t/n v/t/n' format)\n");

			for (int i = 0; i < 3; ++i)
			{
				vertexIndices.push_back(vertexIndex[i]);
				normalIndices.push_back(normalIndex[i]);
				uvIndices.push_back(uvIndex[i]);
			}
		}
	}
}

//----------------------------------------------------------------------------------

static bool LoadOBJModel(const std::string& path, Model model)
{
	ModelImpl* impl = (ModelImpl*)model.get();

	FILE* file = fopen(path.c_str(), "rb");
	bool loaded = (NULL != file);

	if (loaded)
	{
		std::vector<unsigned int> vertexIndices;
		std::vector<unsigned int> normalIndices;
		std::vector<unsigned int> uvIndices;
		std::vector<glm::vec3> vertexList;
		std::vector<glm::vec3> normalList;
		std::vector<glm::vec2> uvList;

		ReadOBJFileData(file, vertexIndices, normalIndices, uvIndices, vertexList, normalList, uvList);

		// A model must consist of position, normal and texture coordinate values...
		impl->vertexArray = CreateVertexArray(VertexPositionNormalTexture::VertexDecl);

		if (impl->vertexArray)
		{
			std::vector<VertexPositionNormalTexture> vertices(vertexIndices.size());

			for (size_t i = 0; i < vertexIndices.size(); ++i)
			{
				const unsigned int vertexIndex = vertexIndices[i];
				const unsigned int normalIndex = normalIndices[i];
				const unsigned int uvIndex = uvIndices[i];

				VertexPositionNormalTexture v;
				v.Position = vertexList[vertexIndex - 1];
				v.Normal = normalList[normalIndex - 1];
				v.TexCoord = uvList[uvIndex - 1];

				vertices.push_back(v);
			}

			impl->vertexCount = vertices.size();

			const size_t bufferSize = impl->vertexCount * VertexPositionNormalTexture::VertexDecl.Stride;
			impl->vertexArray->Vertices = CreateVertexBuffer(BufferUsageHint::Static, bufferSize);

			if (impl->vertexArray->Vertices)
			{
				impl->vertexArray->Vertices->SetData(vertices.data(), bufferSize);
			}
			else
			{
				loaded = false;
			}
		}
		else
		{
			loaded = false;
		}

		fclose(file);
	}

	return loaded;
}

//----------------------------------------------------------------------------------

static bool MatchString(const char* const a, const char* const b, bool caseSensitive)
{
	bool same = true;

	if (caseSensitive)
	{
		for (unsigned int i = 0; same && (a[i] != '\0') && (b[i] != '\0'); ++i)
		{
			same = tolower(a[i]) == tolower(b[i]);
		}
	}
	else
	{
		for (unsigned int i = 0; same && (a[i] != '\0') && (b[i] != '\0'); ++i)
		{
			same = a[i] == b[i];
		}
	}

	return same;
}
