#include <buffers.h>

bool NewVertexBuffer(size_t size, const void* const data, GLuint* vb)
{
	glGenBuffers(1, vb);
	glBindBuffer(GL_ARRAY_BUFFER, *vb);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return true;
}

//-----------------------------------------------------------------------

StaticBuffer::StaticBuffer()
{
}

StaticBuffer::~StaticBuffer()
{
	glDeleteBuffers(1, &id);
}

boost::shared_ptr<StaticBuffer> StaticBuffer::New()
{
	boost::shared_ptr<StaticBuffer> buffer(new StaticBuffer());
	if (buffer)
	{
		glGenBuffers(1, &buffer->id);
	}
	return buffer;
}

void StaticBuffer::Apply() const
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void StaticBuffer::SetData(const void* const data, size_t dataSize)
{
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
