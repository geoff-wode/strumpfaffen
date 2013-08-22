#include <core/vertexbuffer.h>
#include <gl_loader/gl_loader.h>

using namespace core;

//---------------------------------------------------------------------------------

struct VertexBuffer::Impl
{
	GLuint bufferHandle;
};

//---------------------------------------------------------------------------------

VertexBuffer::VertexBuffer(size_t size, BufferUsage::Enum usage, boost::shared_ptr<VertexDeclaration> vertexDecl)
	: Size(size), Usage(usage), VertexDecl(vertexDecl),
		impl(new Impl())
{
	glGenBuffers(1, &impl->bufferHandle);
	Activate();
	glBufferData(GL_ARRAY_BUFFER, size, NULL, (usage == BufferUsage::Dynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	Deactivate();
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &impl->bufferHandle);
}

//---------------------------------------------------------------------------------

void VertexBuffer::Set(const void* const data, size_t size, size_t offset)
{
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void VertexBuffer::Get(size_t size, size_t offset, void* const data)
{
	glGetBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

//---------------------------------------------------------------------------------

void VertexBuffer::Activate()
{
	glBindBuffer(GL_ARRAY_BUFFER, impl->bufferHandle);
}

void VertexBuffer::Deactivate()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
