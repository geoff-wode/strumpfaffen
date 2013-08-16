#include <gl_loader/gl_loader.h>
#include <buffer.h>

//-------------------------------------------------------------------

Buffer::Buffer(size_t size, GLenum type, GLenum usage)
	: type(type)
{
	glGenBuffers(1, &handle);
	Bind();
	glBufferData(type, size, NULL, usage);
	Unbind();
}

Buffer::~Buffer()
{
	glDeleteBuffers(1, &handle);
}

GLuint Buffer::GetHandle() const
{
	return handle;
}

void Buffer::SetData(const void* const data, size_t size, size_t offset)
{
	glBufferSubData(type, offset, size, data);
}

void Buffer::GetData(void* const data, size_t size, size_t offset)
{
	glGetBufferSubData(type, offset, size, data);
}

void Buffer::Bind()
{
	glBindBuffer(type, handle);
}

void Buffer::Unbind()
{
	glBindBuffer(type, 0);
}
