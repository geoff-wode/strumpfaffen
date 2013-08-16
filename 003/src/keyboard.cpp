#include <keyboard.h>

std::list<SDL_Scancode> Keyboard::pressedKeys(SDL_NUM_SCANCODES);

bool Keyboard::IsKeyDown(SDL_Scancode key)
{
	return (0 != SDL_GetKeyboardState(NULL)[key]);
}

bool Keyboard::IsKeyUp(SDL_Scancode key)
{
	return (0 == SDL_GetKeyboardState(NULL)[key]);
}

void Keyboard::GetPressedKeys(std::list<SDL_Scancode>& keys)
{
	keys.clear();
	const Uint8* keyState = SDL_GetKeyboardState(NULL);
	for (int c = 0; c < SDL_NUM_SCANCODES; ++c)
	{
		if (keyState[c]) { keys.push_back((SDL_Scancode)c); }
	}
}
