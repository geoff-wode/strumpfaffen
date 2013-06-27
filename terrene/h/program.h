#if ! defined(__PROGRAM_H__)
#define __PROGRAM_H__

#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <terrene/h/graphicsdevice.h>

namespace Terrene
{

	class Program : public boost::noncopyable
	{
	public:
		Program();
		virtual ~Program();

		void Run();
		void Stop();

	protected:
		virtual void Initialise();
		virtual void Update(float elapsedMS);
		virtual void Draw(float elapsedMS);
		virtual void Stopping();

		const boost::scoped_ptr<GraphicsDevice> Device;

	private:
		void UpdateFrame(float elapsedMS);
		void DrawFrame(float elapsedMS);

		bool running;
	};

}

#endif // __PROGRAM_H__
