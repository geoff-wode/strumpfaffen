#if ! defined(__VERTEX_DECLARATION_H__)
#define __VERTEX_DECLARATION_H__

#include <boost/scoped_ptr.hpp>
#include <vertextypes/vertexelement.h>

namespace Framework
{

	class VertexDeclaration
	{
	public:
		VertexDeclaration(size_t numElements, VertexElementArray elements);

		size_t VertexStride() const;
		size_t GetElementCount() const;
		VertexElementArray GetElements();

	private:
		size_t elementCount;
		VertexElementArray elementList;
		size_t vertexStride;
	};

}

#endif // __VERTEX_DECLARATION_H__
