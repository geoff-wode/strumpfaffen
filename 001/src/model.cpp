#include <debug.h>
#include <model.h>
#include <gl/gl_loader.h>
#include <glm/glm.hpp>
#include <glm/gtc/half_float.hpp>
#include <vector>
#include <stdio.h>
#include <string>

//--------------------------------------------------------

struct Vertex
{
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
};

//--------------------------------------------------------

static bool LoadOBJFile(const std::string& filename, std::vector<Vertex>& vertices);
static bool LoadFile(const std::string& filename, std::string& content);

//--------------------------------------------------------

Model::Model()
	: dataBuffer(NULL), vao(0), vertexCount(0)
{
}

Model::~Model()
{
	glDeleteVertexArrays(1, &vao);
}

//--------------------------------------------------------

boost::shared_ptr<Model> Model::New()
{
	boost::shared_ptr<Model> model(new Model());

	if (model)
	{
		glGenVertexArrays(1, &model->vao);
		model->dataBuffer = StaticBuffer::New();
		if (!model->dataBuffer)
		{
			model.reset();
		}
	}

	return model;
}

//--------------------------------------------------------

bool Model::Load(const std::string& filename)
{
	std::vector<Vertex> vertices;
	const std::string fullPath("models\\" + filename);
	const bool loaded = LoadOBJFile(fullPath, vertices);

	if (loaded)
	{
		vertexCount = vertices.size();

		const size_t dataSize = vertices.size() * sizeof(Vertex);
		dataBuffer->SetData(vertices.data(), dataSize);

		glBindVertexArray(vao);
		dataBuffer->Apply();
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uv));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, normal));
		glBindVertexArray(0);
	}

	return loaded;
}

void Model::Render() const
{
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	glBindVertexArray(0);
}

//--------------------------------------------------------

static bool LoadOBJFile(const std::string& filename, std::vector<Vertex>& vertices)
{
	std::vector<unsigned int> vertexIndices;
	std::vector<unsigned int> normalIndices;
	std::vector<unsigned int> uvIndices;
	std::vector<glm::vec3> vertexList;
	std::vector<glm::vec3> normalList;
	std::vector<glm::vec2> uvList;

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
			vertexList.push_back(glm::vec3(x, y, z));
		}
		else if (0 == strcmp("vt", lineHeader))
		{
			float u, v;
			fscanf(in, "%f %f\n", &u, &v);
			uvList.push_back(glm::vec2(u, v));
		}
		else if (0 == strcmp("vn", lineHeader))
		{
			float x, y, z;
			fscanf(in, "%f %f %f\n", &x, &y, &z);
			normalList.push_back(glm::vec3(x, y, z));
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
				vertexIndices.push_back(vertexIndex[i]);
				normalIndices.push_back(normalIndex[i]);
				uvIndices.push_back(uvIndex[i]);
			}
		}
	}

	vertices.resize(vertexIndices.size());
	for (size_t i = 0; i < vertexIndices.size(); ++i)
	{
		const unsigned int vertexIndex = vertexIndices[i];
		const unsigned int normalIndex = normalIndices[i];
		const unsigned int uvIndex = uvIndices[i];

		Vertex v;
		v.position = vertexList[vertexIndex - 1];
		v.normal = normalList[normalIndex - 1];
		v.uv = uvList[uvIndex - 1];

		vertices.push_back(v);
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
