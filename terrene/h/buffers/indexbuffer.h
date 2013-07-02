#if ! defined(__INDEX_BUFFER_H__)
#define __INDEX_BUFFER_H__

#include <boost/shared_ptr.hpp>
#include <terrene/h/buffers/bufferusagehint.h>
#include <terrene/h/buffers/buffer.h>

namespace Terrene
{
	class IIndexBuffer : public IBuffer
	{
	public:
		static void Deactivate();
	};

	typedef boost::shared_ptr<IIndexBuffer> IndexBuffer;

	IndexBuffer	CreateIndexBuffer(BufferUsageHint::Enum usage, size_t sizeInBytes);
}

#endif // __INDEX_BUFFER_H__
