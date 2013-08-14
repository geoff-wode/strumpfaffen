#include <gl_loader/gl_loader.h>
#include <boost/shared_ptr.hpp>
#include <buffers/buffer.h>
#include <util/debug.h>

using namespace Graphics;

//---------------------------------------------------------------------------------

struct BufferImpl : public IBuffer
{
	GLuint handle;
	size_t size;
	GLenum type;
	GLenum usage;

	virtual void SetData(const void* const data, size_t size, size_t offset)
	{
		Enable();
		glBufferSubData(type, offset, size, data);
		Disable();
	}

	virtual void GetData(void* const data, size_t size, size_t offset) const
	{
		Enable();
		glGetBufferSubData(type, offset, size, data);
		Disable();
	}

	virtual void Enable() const { glBindBuffer(type, handle); }

	virtual void Disable() const { glBindBuffer(type, 0); }

	virtual GLuint GetHandle() const { return handle ; }
	virtual size_t GetSize() const { return size; }
};

//---------------------------------------------------------------------------------

Buffer Graphics::CreateBuffer(BufferType::Enum type, size_t size, BufferUsageHint::Enum usage)
{
	boost::shared_ptr<BufferImpl> buffer(new BufferImpl());

	buffer->size = size;

	switch (type)
	{
	case BufferType::Index: buffer->type = GL_ELEMENT_ARRAY_BUFFER; break;
	case BufferType::Vertex: buffer->type = GL_ARRAY_BUFFER; break;
	case BufferType::ShaderUniform: buffer->type = GL_UNIFORM_BUFFER; break;
	default: ASSERT(false);
	}

	switch (usage)
	{
	case BufferUsageHint::Static: buffer->usage = GL_STATIC_DRAW; break;
	case BufferUsageHint::Dynamic: buffer->usage = GL_DYNAMIC_DRAW; break;
	case BufferUsageHint::Stream: buffer->usage = GL_STREAM_DRAW; break;
	}

	buffer->Enable();
	glBufferData(buffer->type, size, NULL, buffer->usage);
	buffer->Disable();

	return buffer;
}
