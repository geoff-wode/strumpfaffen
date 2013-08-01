#if ! defined(__MODEL_H__)
#define __MODEL_H__

#include <string>
#include <boost/shared_ptr.hpp>

namespace Geometry
{
	class IModel
	{
	public:
		virtual void Render() = 0;

	protected:
		~IModel() { }
	};

	typedef boost::shared_ptr<IModel> Model;

	Model LoadModel(const std::string& filename);

}

#endif // __MODEL_H__
