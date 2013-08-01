#include <gl_loader/gl_loader.h>
#include <buffers/indexbuffer.h>
#include <util/debug.h>

using namespace Buffers;

//-----------------------------------------------------------------------

class IndexBufferImpl : public IIndexBuffer
{
public:
	IndexBufferImpl(IndexBufferType::Enum dataType, size_t count, UsageHint::Enum usageHint)
		: dataType(dataType), elementCount(count), usageHint(usageHint)
	{
		const size_t dataStride = dataType;
		const size_t dataLength = dataStride * count;

		glGenBuffers(1, &handle);
		Activate();
		switch (usageHint)
		{
		case UsageHint::Static: glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataLength, NULL, GL_STATIC_DRAW); break;
		case UsageHint::Dynamic: glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataLength, NULL, GL_DYNAMIC_DRAW); break;
		case UsageHint::Streaming: glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataLength, NULL, GL_STREAM_DRAW); break;
		}
		Deactivate();
	}

	virtual ~IndexBufferImpl()
	{
		glDeleteBuffers(1, &handle);
	}

	virtual void SetData(const void* const data, size_t count, size_t startIndex) const
	{
		ASSERT(count + startIndex <= elementCount);

		const size_t dataStride = dataType;
		const size_t offset = dataStride * startIndex;
		const size_t dataLength = dataStride * count;

		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, dataLength, data);
	}

	virtual void GetData(void* const data, size_t count, size_t startIndex) const
	{
		ASSERT(count + startIndex <= elementCount);

		const size_t dataStride = dataType;
		const size_t offset = dataStride * startIndex;
		const size_t dataLength = dataStride * count;

		glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, dataLength, data);
	}

	virtual void Activate() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
	}

	virtual void Deactivate() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	virtual size_t ElementCount() const
	{
		return elementCount;
	}

	virtual IndexBufferType::Enum ElementType() const
	{
		return dataType;
	}

private:
	IndexBufferType::Enum dataType;
	size_t elementCount;
	UsageHint::Enum usageHint;
	GLuint handle;
};

//-----------------------------------------------------------------------

IndexBuffer Buffers::CreateIndexBuffer(IndexBufferType::Enum dataType, size_t count, UsageHint::Enum usageHint)
{
	IndexBuffer ib(new IndexBufferImpl(dataType, count, usageHint));
	return ib;
}
