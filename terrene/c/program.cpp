#include <SDL.h>
#include <program.h>
#include <inputmanager.h>
#include <debug.h>
#include <time.h>

using namespace Terrene;

Program::Program()
	: running(true), Device(new GraphicsDevice())
{
}

Program::~Program()
{
}

void Program::Run()
{
	{
		time_t now;
		time(&now);
		struct tm* info = localtime(&now);
		LOG("started at %s", asctime(info));
	}

	SDL_Init(SDL_INIT_EVERYTHING);
	atexit(SDL_Quit);

	// initialise sub-systems
	Device->Initialise();
	InputManager::Initialise();

	// initialise program class
	Initialise();

	// frame timing vars
	const float targetFrameLength = 1000.0f / 60.0f;
	float previousFrameLength = 0.0f;

	// main loop
	while (running)
	{
		// compute elapsed time in ms since start of last frame
		const float now = (float)SDL_GetTicks();
		float elapsedMS = (now - previousFrameLength) / 1000.0f;
		if (elapsedMS > targetFrameLength) { elapsedMS = targetFrameLength; }
		previousFrameLength = now;

		// update
		InputManager::Update(*this);	// input
		UpdateFrame(elapsedMS);				// program

		// render
		if (running)
		{
			DrawFrame(elapsedMS);
		}
	}

	// shut down sub-systems
}

void Program::Stop()
{
	running = false;
}

void Program::UpdateFrame(float elapsedMS)
{
	Update(elapsedMS);
}

void Program::DrawFrame(float elapsedMS)
{
	Draw(elapsedMS);
	SDL_GL_SwapBuffers();
}

void Program::Initialise()
{
}

void Program::Update(float elapsedMS)
{
}

void Program::Draw(float elapsedMS)
{
}

void Program::Stopping()
{
}
