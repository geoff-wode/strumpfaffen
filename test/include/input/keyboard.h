#if ! defined(__INPUT_KEYBOARD__)
#define __INPUT_KEYBOARD__

#include <SDL.h>
#include <memory>

class KeyState
{
public:
	KeyState()
	{
		std::memcpy(pressedKeys, SDL_GetKeyboardState(NULL), sizeof(pressedKeys));
	}

	bool IsDown(SDL_Scancode key) const { return (0 != pressedKeys[key]); }
	bool IsUp(SDL_Scancode key)		const { return (0 == pressedKeys[key]); }

private:
	Uint8 pressedKeys[SDL_NUM_SCANCODES];
};

class Keyboard
{
public:
	static KeyState GetKeyState()
	{
		KeyState state;
		return state;
	}
};

#endif // __INPUT_KEYBOARD__
