#if ! defined(__VERTEX_ARRAY_H__)
#define __VERTEX_ARRAY_H__

#include <vector>
#include <boost/shared_ptr.hpp>
#include <terrene/h/buffers/vertexbuffer.h>
#include <terrene/h/buffers/indexbuffer.h>
#include <terrene/h/vertextypes.h>
#include <gl_loader/gl_loader.h>

namespace Terrene
{
	class IVertexArray
	{
	public:
		VertexBuffer	Vertices;
		IndexBuffer		Indices;

		virtual void Activate() = 0;

	protected:
		~IVertexArray() { }
	};

	typedef boost::shared_ptr<IVertexArray> VertexArray;

	VertexArray	CreateVertexArray(const VertexDeclaration& vertexDeclaration);
}

#endif // __VERTEX_ARRAY_H__
