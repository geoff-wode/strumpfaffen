#include <buffers/vertexbuffer.h>
#include <gl_loader/gl_loader.h>

using namespace Buffers;

struct VertexBufferImpl : public IVertexBuffer
{
	size_t size;
	GLuint handle;

	~VertexBufferImpl()
	{
		glDeleteBuffers(1, &handle);
	}

	virtual void SetData(const void* const data, size_t size, size_t offset)
	{
		Enable();
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
		Disable();
	}

	virtual void GetData(void* const data, size_t size, size_t offset) const
	{
		Enable();
		glGetBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
		Disable();
	}

	virtual void Enable() const { glBindBuffer(GL_ARRAY_BUFFER, handle); }

	virtual void Disable() const{ glBindBuffer(GL_ARRAY_BUFFER, 0); }

	virtual size_t Size() const { return size; }
};

VertexBuffer Buffers::CreateVertexBuffer(size_t size, Buffers::UsageHint::Enum usage)
{
	boost::shared_ptr<VertexBufferImpl> vb(new VertexBufferImpl());
	vb->Enable();
	switch (usage)
	{
	case Buffers::UsageHint::Static: glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW); break;
	case Buffers::UsageHint::Dynamic: glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW); break;
	case Buffers::UsageHint::Streaming: glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STREAM_DRAW); break;
	}
	vb->Disable();
	return vb;
}
