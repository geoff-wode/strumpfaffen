#include <buffers/indexbuffer.h>
#include <gl_loader/gl_loader.h>
#include <debug.h>

using namespace Terrene;

void IIndexBuffer::Deactivate()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

class IndexBufferImpl : public IIndexBuffer
{
public:
	IndexBufferImpl() { glGenBuffers(1, &handle); }

	virtual ~IndexBufferImpl() { glDeleteBuffers(1, &handle); }

	virtual void SetData(const void* const data, size_t sizeInBytes)
	{
		SetData(data, 0, sizeInBytes);
	}

	virtual void SetData(const void* const data, size_t offset, size_t sizeInBytes)
	{
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, sizeInBytes, data);
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
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
	}

private:
	GLuint handle;
};

//-----------------------------------------------------------------------------

IndexBuffer CreateIndexBuffer(BufferUsageHint::Enum hint, size_t sizeInBytes)
{
	IndexBuffer ib(new IndexBufferImpl());

	if (ib)
	{
		ib->Activate();
		GLenum bufferHint;
		switch (hint)
		{
		case BufferUsageHint::Dynamic: bufferHint = GL_DYNAMIC_DRAW; break;
		case BufferUsageHint::Static:  bufferHint = GL_STATIC_DRAW;  break;
		case BufferUsageHint::Stream:  bufferHint = GL_STREAM_DRAW;  break;
		default: ASSERTM(false, "invalid usage hint\n"); break;
		}
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeInBytes, NULL, bufferHint);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	return ib;
}
