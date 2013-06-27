#if ! defined(__VERTEX_BUFFER_H__)
#define __VERTEX_BUFFER_H__

#include <boost/shared_ptr.hpp>
#include <terrene/h/bufferusagehint.h>

namespace Terrene
{

	class IVertexBuffer
	{
	public:
		// Copy 'sizeInBytes' system data into the buffer from 'data' (assumed to be large enough!).
		virtual void SetData(const void* const data, size_t sizeInBytes) = 0;

		// Copy 'sizeInBytes' system data into the buffer at 'offset' from 'data' (assumed to be large enough!).
		virtual void SetData(const void* const data, size_t sizeInBytes, size_t offset) = 0;

		// Copy 'sizeInBytes' from the buffer into 'data' (assumed to be large enough!).
		virtual void GetData(size_t sizeInBytes, void* const data) = 0;

		// Copy 'sizeInBytes' from the buffer starting at 'offset' into 'data' (assumed to be large enough!).
		virtual void GetData(size_t sizeInBytes, size_t offset, void* const data) = 0;

		// Make this vertex buffer active.
		virtual void Activate() const = 0;

	protected:
		~IVertexBuffer() { }
	};

	typedef boost::shared_ptr<IVertexBuffer> VertexBuffer;

	VertexBuffer CreateVertexBuffer(BufferUsageHint::Enum hint, size_t sizeInBytes);

}

#endif // __VERTEX_BUFFER_H__
