#include <buffers.h>

Buffer::Buffer(GLenum type, size_t size, GLenum usage)
	: type(type)
{
	glGenBuffers(1, &handle);
	glBindBuffer(type, handle);
	glBufferData(type, size, NULL, usage);
	glBindBuffer(type, 0);
}

Buffer::~Buffer()
{
	glDeleteBuffers(1, &handle);
}

void Buffer::SetData(const void* const data, size_t size, size_t offset)
{
	glBufferSubData(type, offset, size, data);
}

void Buffer::GetData(void* const data, size_t size, size_t offset)
{
	glGetBufferSubData(type, offset, size, data);
}

void Buffer::Enable()
{
	glBindBuffer(type, handle);
}

//----------------------------------------------------------------------------------

boost::shared_ptr<IndexBuffer> CreateIndexBuffer(size_t size, GLenum type, GLenum usage)
{
	return boost::make_shared<IndexBuffer>(size, type, usage);
}

boost::shared_ptr<VertexBuffer> CreateVertexBuffer(size_t size, GLenum usage)
{
	return boost::make_shared<VertexBuffer>(size, usage);
}

boost::shared_ptr<UniformBuffer> CreateUniformBuffer(size_t size, GLenum usage)
{
	return boost::make_shared<UniformBuffer>(size, usage);
}
