#include <SDL.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include <util/debug.h>
#include <core/program.h>

using namespace Core;

//---------------------------------------------------------------

class Program::ProgramImpl
{
public:
	ProgramImpl()
		: quit(false),
			lastUpdateTime(0),
			lastDrawTime(0)
	{
	}

	bool AppIsActive() const
	{
		return SDL_APPACTIVE == (SDL_GetAppState() & SDL_APPACTIVE);
	}

	void ManageEvents()
	{
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT: quit = true; break;
			}
		}
	}

	bool quit;
	unsigned int lastUpdateTime;
	unsigned int lastDrawTime;
	static const unsigned int TargetFrameSpeedMS = 1000 / 60;
};

//---------------------------------------------------------------

Program::Program()
	: impl(new ProgramImpl()),
		Device(new Core::Device())
{
}

Program::~Program()
{
}

void Program::Run()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	atexit(SDL_Quit);

	time_t now = time(NULL);
	struct tm* utcTime = localtime(&now);
	char buffer[128] = { 0 };
	strftime(buffer, sizeof(buffer)-1, "%Y-%m-%d at %H:%M:%S", utcTime);
	LOG("System start: %s\n", buffer);

	if (!Device->Initialise()) { return; }

	Initialise();
	LoadResources();

	while (!impl->quit)
	{
		impl->ManageEvents();

		if (impl->AppIsActive())
		{
			// Update the program state...
			const unsigned int now = SDL_GetTicks();
			Update(now - impl->lastUpdateTime);
			impl->lastUpdateTime = now;

			// Work out if it's time to render the next frame...
			if ((now - impl->lastDrawTime) > impl->TargetFrameSpeedMS)
			{
				Draw();
				impl->lastDrawTime = now;
				SDL_GL_SwapBuffers();
			}
		}
	}

	Quitting();
}

void Program::Quit()
{
	impl->quit = true;
}

void Program::Initialise()
{
}

void Program::LoadResources()
{
}

void Program::Update(unsigned int elapsedMS)
{
}

void Program::Draw()
{
}

void Program::Quitting()
{
}
