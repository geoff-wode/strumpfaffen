#if ! defined(__VERTEX_BUFFER_H__)
#define __VERTEX_BUFFER_H__

#include <boost/shared_ptr.hpp>
#include <buffers/bufferusagehint.h>
#include <vertextypes/vertexdeclaration.h>

namespace Buffers
{

	class IVertexBuffer
	{
	public:
		virtual void SetData(const void* const data, size_t vertexCount, size_t offset) const = 0;

		virtual void GetData(void* const data, size_t vertexCount, size_t offset) const = 0;

		virtual void Activate() const = 0;
		virtual void Deactivate() const = 0;

		// Returns the maximum number of vertices that can be stored in the buffer.
		virtual size_t VertexCount() const = 0;

		// Returns the vertex declaration associated with this buffer.
		virtual const Framework::VertexDeclaration& VertexDeclaration() const = 0;

	protected:
		~IVertexBuffer();
	};

	typedef boost::shared_ptr<IVertexBuffer> VertexBuffer;

	VertexBuffer CreateVertexBuffer(
		const Framework::VertexDeclaration& vertexDeclaration,
		size_t vertexCount,
		UsageHint::Enum usageHint);

}

#endif // __VERTEX_BUFFER_H__
