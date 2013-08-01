#if ! defined(__KEYBOARD_H__)
#define __KEYBOARD_H__

#include <input/keystate.h>

namespace Input
{

	class Keyboard
	{
	public:
		static KeyState GetKeyState();
	};

}

#endif // __KEYBOARD_H__
