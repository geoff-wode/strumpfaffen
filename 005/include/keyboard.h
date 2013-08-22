#if ! defined(__INPUT_KEYBOARD__)
#define __INPUT_KEYBOARD__

#include <SDL.h>
#include <boost/shared_ptr.hpp>

class IKeyboardState
{
public:
	virtual bool IsKeyDown(SDL_Scancode key) const = 0;
	virtual bool IsKeyUp(SDL_Scancode key) const = 0;
};

typedef boost::shared_ptr<IKeyboardState> KeyboardState;

class Keyboard
{
public:
	static KeyboardState GetState();
};

#endif // __INPUT_KEYBOARD__
