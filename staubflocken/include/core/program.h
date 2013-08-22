#if ! defined(__PROGRAM__)
#define __PROGRAM__

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <core/device.h>

namespace core
{
	class Program
	{
	public:
		Program();
		virtual ~Program();
		void Run();
		void Quit();

	protected:
		virtual bool Initialise();
		virtual void LoadResources();
		virtual void Update(unsigned int elapsedMS);
		virtual void Render();
		virtual void Quitting();

		boost::shared_ptr<Device> Device;

	private:
		bool quit;

		void HandleEvents();
	};
}

#endif // __PROGRAM__
