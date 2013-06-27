#if ! defined(__INPUT_MANAGER_H__)
#define __INPUT_MANAGER_H__

#include <list>
#include <SDL.h>
#include <terrene/h/program.h>

namespace Terrene
{

	struct KeyboardState
	{
		std::list<SDLKey> PressedKeys;

		bool IsKeyDown(SDLKey key) const;
		bool IsKeyUp(SDLKey key) const;
	};

	namespace InputManager
	{
		void Initialise();
		void Update(Program& program);

		KeyboardState GetKeyboardState();
	};

}

#endif // __INPUT_MANAGER_H__
