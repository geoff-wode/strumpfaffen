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
	Bind();
	glBufferSubData(type, offset, size, data);
	Unbind();
}

void Buffer::GetData(void* const data, size_t size, size_t offset)
{
	Bind();
	glGetBufferSubData(type, offset, size, data);
	Unbind();
}

void Buffer::Bind()
{
	glBindBuffer(type, handle);
}

void Buffer::Unbind()
{
	glBindBuffer(type, 0);
}
