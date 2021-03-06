#include <gl_loader/gl_loader.h>
#include <buffers/vertexbuffer.h>
#include <debug.h>

using namespace Terrene;

void IVertexBuffer::Deactivate()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

class VertexBufferImpl : public IVertexBuffer
{
public:
	VertexBufferImpl() { glGenBuffers(1, &handle); }
	virtual ~VertexBufferImpl() { glDeleteBuffers(1, &handle); }

	// Copy 'sizeInBytes' system data into the buffer from 'data' (assumed to be large enough!).
	virtual void SetData(const void* const data, size_t sizeInBytes)
	{
		SetData(data, sizeInBytes, 0);
	}

	// Copy 'sizeInBytes' system data into the buffer at 'offset' from 'data' (assumed to be large enough!).
	virtual void SetData(const void* const data, size_t sizeInBytes, size_t offset)
	{
		glBufferSubData(GL_ARRAY_BUFFER, offset, sizeInBytes, data);
	}

	// Copy 'sizeInBytes' from the buffer into 'data' (assumed to be large enough!).
	virtual void GetData(size_t sizeInBytes, void* const data)
	{
		GetData(sizeInBytes, 0, data);
	}

	// Copy 'sizeInBytes' from the buffer starting at 'offset' into 'data' (assumed to be large enough!).
	virtual void GetData(size_t sizeInBytes, size_t offset, void* const data)
	{
		glGetBufferSubData(GL_ARRAY_BUFFER, offset, sizeInBytes, data);
	}

	virtual void Activate() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, handle);
	}

private:
	GLuint handle;
};

//-----------------------------------------------------------------------------

VertexBuffer Terrene::CreateVertexBuffer(BufferUsageHint::Enum hint, size_t sizeInBytes)
{
	VertexBuffer vb(new VertexBufferImpl());

	if (vb)
	{
		vb->Activate();
		GLenum bufferHint;
		switch (hint)
		{
		case BufferUsageHint::Dynamic: bufferHint = GL_DYNAMIC_DRAW; break;
		case BufferUsageHint::Static:  bufferHint = GL_STATIC_DRAW;  break;
		case BufferUsageHint::Stream:  bufferHint = GL_STREAM_DRAW;  break;
		default: ASSERTM(false, "invalid usage hint\n"); break;
		}
		glBufferData(GL_ARRAY_BUFFER, sizeInBytes, NULL, bufferHint);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	return vb;
}
