#if ! defined(__INPUT_MOUSE__)
#define __INPUT_MOUSE__

#include <SDL.h>
#include <glm/glm.hpp>

struct MouseState
{
	MouseState()
	{
		buttons = SDL_GetMouseState(&position.x, &position.y);
	}

	bool IsButtonDown(unsigned int button) const { return (buttons & button); }
	bool IsButtonUp(unsigned int button) const { return !(buttons & button); }

	Uint32			buttons;
	glm::ivec2	position;
};

class Mouse
{
public:
	static MouseState GetMouseState()
	{
		MouseState state;
		return state;
	}
};

#endif // __INPUT_MOUSE__
