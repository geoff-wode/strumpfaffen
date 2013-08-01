#if ! defined(__VERTEX_ARRAY_H__)
#define __VERTEX_ARRAY_H__

#include <vector>
#include <boost/shared_ptr.hpp>
#include <buffers/indexbuffer.h>
#include <buffers/vertexbuffer.h>
#include <vertextypes/vertexdeclaration.h>

using namespace Buffers;

namespace VertexArrays
{

	class IVertexArray
	{
	public:
		virtual VertexBuffer GetVertexBuffer() const = 0;
		virtual IndexBuffer GetIndexBuffer() const = 0;

		virtual void Activate() const = 0;
		virtual void Deactivate() const = 0;

	protected:
		virtual ~IVertexArray() { }
	};

	typedef boost::shared_ptr<IVertexArray> VertexArray;

	VertexArray CreateVertexArray(VertexBuffer vb);

	VertexArray CreateVertexArray(VertexBuffer vb, IndexBuffer ib);
}

#endif // __VERTEX_ARRAY_H__
