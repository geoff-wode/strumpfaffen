#if ! defined(__KEYBOARD__)
#define __KEYBOARD__

#include <SDL.h>
#include <vector>

class Keyboard
{
public:
	static bool IsKeyDown(SDLKey key);
	static bool IsKeyUp(SDLKey key);

	static void GetPressedKeys(std::vector<SDLKey>& keys);
};

#endif //__KEYBOARD__
