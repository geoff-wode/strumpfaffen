#if ! defined(__VERTEX_ARRAY__)
#define __VERTEX_ARRAY__

#include <buffer.h>
#include <geometry/vertexdeclaration.h>
#include <boost/shared_ptr.hpp>

struct VertexArray
{
	const VertexDeclaration* vertexDecl;
	boost::shared_ptr<Buffer> vertexBuffer;
	boost::shared_ptr<Buffer> indexBuffer;
};

#endif //__VERTEX_ARRAY__
