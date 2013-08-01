#include <memory>
#include <input/keyboard.h>

using namespace Input;

//-----------------------------------------------------------------------------

KeyState Keyboard::GetKeyState()
{
	KeyState state;
	std::memcpy(state.keys, SDL_GetKeyState(NULL), SDLK_LAST-1);
	return state;
}

//-----------------------------------------------------------------------------

KeyState::KeyState()
{
	std::memset(keys, 0, sizeof(keys));
}

KeyState::KeyState(const KeyState& other)
{
	std::memcpy(keys, other.keys, sizeof(keys));
}

bool KeyState::IsKeyDown(SDLKey key) const
{
	return (0 != keys[key]);
}

bool KeyState::IsKeyUp(SDLKey key) const
{
	return (0 == keys[key]);
}
