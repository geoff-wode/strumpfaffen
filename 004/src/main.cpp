#include <SDL.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <core/debug.h>
#include <core/device.h>
#include <core/program.h>

//----------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	LOG("\n");
	
	boost::shared_ptr<core::Device> device = boost::make_shared<core::Device>();
	device->BackbufferWidth = 800;
	device->BackbufferHeight = 600;

	core::Program program(device);
	program.Run();

	return 0;
}
