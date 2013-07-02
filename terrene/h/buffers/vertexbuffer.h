#if ! defined(__VERTEX_BUFFER_H__)
#define __VERTEX_BUFFER_H__

#include <boost/shared_ptr.hpp>
#include <terrene/h/buffers/bufferusagehint.h>
#include <terrene/h/buffers/buffer.h>

namespace Terrene
{

	class IVertexBuffer : public IBuffer
	{
	public:
		static void Deactivate();
	};

	typedef boost::shared_ptr<IVertexBuffer> VertexBuffer;

	VertexBuffer CreateVertexBuffer(BufferUsageHint::Enum usage, size_t sizeInBytes);

}

#endif // __VERTEX_BUFFER_H__
