#include <gl_loader/gl_loader.h>
#include <buffers/vertexbuffer.h>
#include <util/debug.h>

using namespace Buffers;

//-----------------------------------------------------------------------

IVertexBuffer::~IVertexBuffer()
{
}

//-----------------------------------------------------------------------

class VertexBufferImpl : public IVertexBuffer
{
public:
	VertexBufferImpl(const Core::VertexDeclaration& vertexDeclaration, size_t vertexCount)
		: vertexCount(vertexCount), vertexDeclaration(vertexDeclaration)
	{
		glGenBuffers(1, &handle);
	}

	virtual ~VertexBufferImpl()
	{
		glDeleteBuffers(1, &handle);
	}

	virtual void SetData(const void* const data, size_t vertexCount, size_t startVertex) const
	{
		ASSERT((vertexCount + startVertex) <= this->VertexCount());

		const size_t vertexStride = vertexDeclaration.VertexStride();
		const size_t offset = startVertex * vertexStride;
		const size_t dataLength = vertexCount * vertexStride;

		Activate();
		glBufferSubData(GL_ARRAY_BUFFER, offset, dataLength, data);
		Deactivate();
	}

	virtual void GetData(void* const data, size_t vertexCount, size_t startVertex) const
	{
		ASSERT((vertexCount + startVertex) <= this->VertexCount());

		const size_t vertexStride = vertexDeclaration.VertexStride();
		const size_t offset = startVertex * vertexStride;
		const size_t dataLength = vertexCount * vertexStride;

		glGetBufferSubData(GL_ARRAY_BUFFER, offset, dataLength, data);
	}

	virtual void Activate() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, handle);
	}

	virtual void Deactivate() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	virtual size_t VertexCount() const
	{
		return vertexCount;
	}

	virtual const Core::VertexDeclaration& VertexDeclaration() const
	{
		return vertexDeclaration;
	}

	size_t vertexCount;
	Core::VertexDeclaration vertexDeclaration;
	GLuint handle;
};

//-----------------------------------------------------------------------

VertexBuffer Buffers::CreateVertexBuffer(
		const Core::VertexDeclaration& vertexDeclaration,
		size_t vertexCount,
		UsageHint::Enum usageHint)
{
	VertexBuffer vb(new VertexBufferImpl(vertexDeclaration, vertexCount));
	const size_t bufferSize = vertexDeclaration.VertexStride() * vertexCount;
	if (vb)
	{
		vb->Activate();
		switch (usageHint)
		{
		case UsageHint::Static: glBufferData(GL_ARRAY_BUFFER, bufferSize, NULL, GL_STATIC_DRAW); break;
		case UsageHint::Dynamic: glBufferData(GL_ARRAY_BUFFER, bufferSize, NULL, GL_DYNAMIC_DRAW); break;
		case UsageHint::Streaming: glBufferData(GL_ARRAY_BUFFER, bufferSize, NULL, GL_STREAM_DRAW); break;
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	return vb;
}
