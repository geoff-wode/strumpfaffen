#include <memory>
#include <keyboard.h>
#include <boost/make_shared.hpp>

//-------------------------------------------------------------------

class KeyboardStateImpl : public IKeyboardState
{
public:
	KeyboardStateImpl()
	{
		std::memcpy(keyState, SDL_GetKeyboardState(NULL), SDL_NUM_SCANCODES);
	}

	virtual bool IsKeyDown(SDL_Scancode key) const { return 0 != keyState[key]; }
	virtual bool IsKeyUp(SDL_Scancode key) const { return 0 == keyState[key]; }

	Uint8 keyState[SDL_NUM_SCANCODES];
};

//-------------------------------------------------------------------

KeyboardState Keyboard::GetState()
{
	return boost::make_shared<KeyboardStateImpl>();
}
