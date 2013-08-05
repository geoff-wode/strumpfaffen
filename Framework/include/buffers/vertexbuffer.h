#if ! defined(__VERTEX_BUFFER_H__)
#define __VERTEX_BUFFER_H__

#include <boost/shared_ptr.hpp>
#include <buffers/bufferusagehint.h>

namespace Buffers
{

	struct IVertexBuffer
	{
		virtual void SetData(const void* const data, size_t size, size_t offset) = 0;
		virtual void GetData(void* const data, size_t size, size_t offset) const = 0;
		virtual void Enable() const = 0;
		virtual void Disable() const = 0;
		virtual size_t Size() const = 0;
	};

	typedef boost::shared_ptr<IVertexBuffer> VertexBuffer;

	VertexBuffer CreateVertexBuffer(size_t size, Buffers::UsageHint::Enum usage);

}

#endif // __VERTEX_BUFFER_H__
