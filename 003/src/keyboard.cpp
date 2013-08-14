#include <keyboard.h>

bool Keyboard::IsKeyDown(SDLKey key)
{
	return (0 != SDL_GetKeyState(NULL)[key]);
}

bool Keyboard::IsKeyUp(SDLKey key)
{
	return (0 == SDL_GetKeyState(NULL)[key]);
}

void Keyboard::GetPressedKeys(std::vector<SDLKey>& keys)
{
	Uint8* keyState = SDL_GetKeyState(NULL);
	for (int k = SDLK_FIRST; k != SDLK_LAST; ++k)
	{
		if (keyState[k]) { keys.push_back((SDLKey)k); }
	}
}
