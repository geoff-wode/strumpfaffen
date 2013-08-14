#if ! defined(__INPUT_KEYBOARD_H__)
#define __INPUT_KEYBOARD_H__

#include <SDL.h>

namespace Framework
{
	namespace Input
	{
		class IKeyboard
		{
		public:
			virtual void OnKeyDown(SDLKey key) = 0;
			virtual void OnKeyUp(SDLKey key) = 0;
		};
	}
}

#endif // __INPUT_KEYBOARD_H__
