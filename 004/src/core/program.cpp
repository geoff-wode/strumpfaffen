#include <SDL.h>
#include <core/program.h>
#include <boost/make_shared.hpp>

using namespace core;

//----------------------------------------------------------------------------------

FILE* errorFile = NULL;

//----------------------------------------------------------------------------------

Program::Program(boost::shared_ptr<core::Device> device)
	: quit(false), Device(device)
{
}

Program::~Program()
{
}

//----------------------------------------------------------------------------------
extern void test();

void Program::Run()
{
	if (!Initialise()) { return; }

	test();

	LoadResources();

	unsigned int lastUpdate = 0;

	while (!quit)
	{
		HandleEvents();

		unsigned int now = SDL_GetTicks();
		unsigned int elapsedMS = now - lastUpdate;
		lastUpdate = now;
		Update(elapsedMS);

		Render();

		Device->SwapBuffers();
	}
}

//----------------------------------------------------------------------------------
void Program::Quit()
{
	quit = true;
	Quitting();
}

//----------------------------------------------------------------------------------
bool Program::Initialise()
{
	return Device->Initialise();
}

//----------------------------------------------------------------------------------
void Program::LoadResources()
{
}

//----------------------------------------------------------------------------------
void Program::Update(unsigned int elapsedMS)
{
}

//----------------------------------------------------------------------------------
void Program::Render()
{
}

//----------------------------------------------------------------------------------
void Program::Quitting()
{
}

//----------------------------------------------------------------------------------
void Program::HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT: quit = true; break;
		default: break;
		}
	}
}
