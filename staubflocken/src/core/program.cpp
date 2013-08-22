#include <SDL.h>
#include <stdlib.h>
#include <core/program.h>
#include <core/device.h>
#include <core/files.h>
#include <boost/make_shared.hpp>

using namespace core;

//----------------------------------------------------------------------------------

namespace staubflocken
{
	FILE* debugLogFile = NULL;
}

const char* debugLogFileName = "stderr.txt";

//----------------------------------------------------------------------------------

Program::Program()
	: quit(false)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	atexit(SDL_Quit);

	staubflocken::debugLogFile = fopen(debugLogFileName, "wb");
}

Program::~Program()
{
}

//----------------------------------------------------------------------------------

void Program::Run()
{
	if (!Initialise()) { return; }

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
