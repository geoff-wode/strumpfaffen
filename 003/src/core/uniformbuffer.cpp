#include <core/uniformbuffer.h>
#include <gl_loader/gl_loader.h>

using namespace core;

//---------------------------------------------------------------------------------

struct UniformBuffer::Impl
{
	GLuint bufferHandle;
};

//---------------------------------------------------------------------------------

UniformBuffer::UniformBuffer(size_t size, BufferUsage::Enum usage, unsigned int bindPoint)
	: Size(size), Usage(usage),
		impl(new Impl())
{
	glGenBuffers(1, &impl->bufferHandle);
	Activate();
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, (usage == BufferUsage::Dynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, bindPoint, impl->bufferHandle);
	Deactivate();
}

UniformBuffer::~UniformBuffer()
{
	glDeleteBuffers(1, &impl->bufferHandle);
}

//---------------------------------------------------------------------------------

void UniformBuffer::Set(const void* const data, size_t size, size_t offset)
{
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

void UniformBuffer::Get(size_t size, size_t offset, void* const data)
{
	glGetBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

//---------------------------------------------------------------------------------

void UniformBuffer::Activate()
{
	glBindBuffer(GL_UNIFORM_BUFFER, impl->bufferHandle);
}

void UniformBuffer::Deactivate()
{
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
