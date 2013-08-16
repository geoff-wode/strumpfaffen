#if ! defined(__MESH__)
#define __MESH__

#include <vector>
#include <geometry/vertexarray.h>
#include <boost/scoped_ptr.hpp>
#include <device.h>

class Mesh
{
public:
	Mesh();
	virtual ~Mesh();

	boost::scoped_ptr<std::vector<VertexArray>> vertexArrays;
};

#endif // __MESH__
