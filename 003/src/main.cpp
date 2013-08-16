#include <SDL.h>
#include <glm/glm.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

#include <debug.h>
#include <device.h>
#include <program.h>

//----------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	LOG("\n");
	Program program(boost::make_shared<Device>(glm::ivec2(800,600)));
	program.Run();
	return 0;
}
