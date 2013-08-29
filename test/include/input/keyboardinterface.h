#if ! defined(__KEYBOARD_INTERFACE__)
#define __KEYBOARD_INTERFACE__

#include <SDL.h>

// Declares an interface which objects that want to get keyboard events
// must implement.
class IKeyboard
{
public:
	virtual void OnKeyDown(SDL_Scancode key) = 0;
	virtual void OnKeyUp(SDL_Scancode key) = 0;
};

#endif // __KEYBOARD_INTERFACE__
