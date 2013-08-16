#if ! defined(__KEYBOARD__)
#define __KEYBOARD__

#include <SDL.h>
#include <list>

class Keyboard
{
public:
	static bool IsKeyDown(SDL_Scancode key);
	static bool IsKeyUp(SDL_Scancode key);

	static void GetPressedKeys(std::list<SDL_Scancode>& keys);

private:
	static std::list<SDL_Scancode> pressedKeys;
};

#endif //__KEYBOARD__
