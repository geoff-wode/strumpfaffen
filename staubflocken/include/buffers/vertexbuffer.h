// A buffer to store a stream of vertex attributes on the GPU.

#if ! defined(__BUFFERS_VERTEXBUFFER__)
#define __BUFFERS_VERTEXBUFFER__

#include <buffers/bufferhint.h>
#include <boost/shared_ptr.hpp>

class VertexBuffer
{
public:
	virtual size_t GetSize() const = 0;
	virtual void SetData(const void* const data, size_t size, size_t offset) = 0;
	virtual void GetData(size_t size, size_t offset, void* const data) = 0;
	virtual void Enable() = 0;
	virtual void Disable() = 0;
};

typedef boost::shared_ptr<VertexBuffer> VertexBufferPtr;

VertexBufferPtr CreateVertexBuffer(size_t size, BufferUsageHint::Enum usage);

#endif // __BUFFERS_VERTEXBUFFER__
