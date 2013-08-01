#if ! defined(__KEY_STATE_H__)
#define __KEY_STATE_H__

#include <SDL.h>

namespace Input
{

	class KeyState
	{
	public:
		KeyState();
		KeyState(const KeyState& other);

		bool IsKeyDown(SDLKey key) const;
		bool IsKeyUp(SDLKey key) const;

	private:
		friend class Keyboard;
		Uint8	keys[SDLK_LAST];
	};

}

#endif // __KEY_STATE_H__
