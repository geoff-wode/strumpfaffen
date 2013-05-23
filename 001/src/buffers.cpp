#include <buffers.h>

bool NewVertexBuffer(size_t size, const void* const data, GLuint* vb)
{
	glGenBuffers(1, vb);
	glBindBuffer(GL_ARRAY_BUFFER, *vb);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return true;
}
