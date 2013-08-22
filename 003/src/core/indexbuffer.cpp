#include <core/indexbuffer.h>
#include <gl_loader/gl_loader.h>

using namespace core;

//---------------------------------------------------------------------------------

struct IndexBuffer::Impl
{
	GLuint bufferHandle;
};

//---------------------------------------------------------------------------------

IndexBuffer::IndexBuffer(size_t size, BufferUsage::Enum usage, IndexBufferDataType::Enum dataType)
	: Size(size), Usage(usage), DataType(dataType),
		impl(new Impl())
{
	glGenBuffers(1, &impl->bufferHandle);
	Activate();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, NULL, (usage == BufferUsage::Dynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	Deactivate();
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &impl->bufferHandle);
}

//---------------------------------------------------------------------------------

void IndexBuffer::Set(const void* const data, size_t size, size_t offset)
{
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}

void IndexBuffer::Get(size_t size, size_t offset, void* const data)
{
	glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}

//---------------------------------------------------------------------------------

void IndexBuffer::Activate()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, impl->bufferHandle);
}

void IndexBuffer::Deactivate()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
