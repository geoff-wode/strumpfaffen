#if ! defined(__MESH_H__)
#define __MESH_H__

#include <boost/shared_ptr.hpp>

namespace Geometry
{
	class IMesh
	{
	public:
		virtual void Render() = 0;

	protected:
		~IMesh() { }
	};

	typedef boost::shared_ptr<IMesh> Mesh;

	Mesh LoadMesh(std::string& name);
}

#endif // __MESH_H__
