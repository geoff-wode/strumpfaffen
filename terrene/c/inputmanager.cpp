#include <algorithm>
#include <inputmanager.h>

using namespace Terrene;

static unsigned int numPressedKeys = 0;
static std::list<SDLKey> pressedKeys(SDLK_LAST);

bool KeyboardState::IsKeyDown(SDLKey key) const
{
	return PressedKeys.end() != std::find(PressedKeys.begin(), PressedKeys.end(), key);
}

bool KeyboardState::IsKeyUp(SDLKey key) const
{
	return PressedKeys.end() == std::find(PressedKeys.begin(), PressedKeys.end(), key);
}

void InputManager::Initialise()
{
	numPressedKeys = 0;
	pressedKeys.clear();
}

void InputManager::Update(Program& program)
{
	pressedKeys.clear();
	numPressedKeys = 0;

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			program.Stop();
			break;
		case SDL_KEYDOWN:
			{
				pressedKeys.push_back(event.key.keysym.sym);
				++numPressedKeys;
			}
			break;
		}
	}
}

KeyboardState InputManager::GetKeyboardState()
{
	KeyboardState state;
	state.PressedKeys = pressedKeys;
	return state;
}
