// A buffer to store a stream of vertex attributes on the GPU.

#if ! defined(__BUFFERS_INDEXBUFFER__)
#define __BUFFERS_INDXBUFFER__

#include <buffers/bufferhint.h>
#include <boost/shared_ptr.hpp>

class IndexBuffer
{
public:
	virtual size_t GetSize() const = 0;
	virtual void SetData(const void* const data, size_t size, size_t offset) = 0;
	virtual void GetData(size_t size, size_t offset, void* const data) = 0;
	virtual void Enable() = 0;
	virtual void Disable() = 0;
};

typedef boost::shared_ptr<IndexBuffer> IndexBufferPtr;

IndexBufferPtr CreateIndexBuffer(size_t size, BufferUsageHint::Enum usage);

#endif // __BUFFERS_INDEXBUFFER__
