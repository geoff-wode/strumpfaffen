
#include <render/mesh.h>

static GLuint ConfigureVAO(size_t attrCount, const VertexAttribute* attrs, VertexBufferPtr vertexBuffer);

Mesh::Mesh(size_t attrCount, const VertexAttribute* attrs, VertexBufferPtr vertexBuffer)
	: vao(ConfigureVAO(attrCount, attrs, vertexBuffer))
{
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &vao);
}

void Mesh::Enable() const { glBindVertexArray(vao); }

static GLuint ConfigureVAO(size_t attrCount, const VertexAttribute* attrs, VertexBufferPtr vertexBuffer)
{
	GLuint vao;

	// Compute the stride from the start of one vertex to the next
	size_t stride = 0;
	for (size_t i = 0; i < attrCount; ++i)
	{
		stride += (attrs[i].elements * sizeof(float));
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Store the vertex buffer into the VAO state...
	vertexBuffer->Enable();

	for (size_t i = 0; i < attrCount; ++i)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, attrs[i].elements, GL_FLOAT, GL_FALSE, stride, (const void*)attrs[i].offset);
	}
	glBindVertexArray(0);

	vertexBuffer->Disable();

	return vao;
}
