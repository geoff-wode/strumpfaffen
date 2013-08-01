#if ! defined(__PROGRAM_H__)
#define __PROGRAM_H__

#include <boost/scoped_ptr.hpp>
#include <core/device.h>

namespace Core
{

	class Program
	{
	public:
		Program();
		virtual ~Program();

		void Run();
		void Quit();

		const boost::scoped_ptr<Core::Device> Device;
	
	protected:
		virtual void Initialise();
		virtual void Update(unsigned int elapsedMS);
		virtual void Draw();
		virtual void Quitting();

	private:
		class ProgramImpl;
		boost::scoped_ptr<ProgramImpl> impl;
	};

}
#endif // __PROGRAM_H__
