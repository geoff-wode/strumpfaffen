#include <SDL.h>
#include <map>
#include <memory>

class KeyboardHandler
{
public:
  KeyboardHandler()
  {
    std::memcpy(prevKeyState, SDL_GetKeyboardState(NULL), SDL_NUM_SCANCODES);
  }

  void MapKey(SDL_Scancode key) { keyMap[key] = key; }

  void Update()
  {
    // Get the current state of the keyboard...
    const Uint8* keyState = SDL_GetKeyboardState(NULL);

    // For each registered key handler, if the key was previously not pressed
    // but now is notify the handler of the change in state.
    // Alternately, if the key was previous pressed but now is not, invoke the
    // handler for that change of state.
    for (KeyMap::iterator it = keyMap.begin(); keyMap.end() != it; ++it)
    {
      if ((0 == prevKeyState[it->first]) && (0 != keyState[it->first]))
      {
        OnKeyPressed(it->first);
      }
      else if ((0 != prevKeyState[it->first]) && (0 == keyState[it->first]))
      {
        OnKeyReleased(it->first);
      }
    }

    // Remember the current state for next time...
    std::memcpy(prevKeyState, keyState, SDL_NUM_SCANCODES);
  }

private:
  typedef std::map<SDL_Scancode, SDL_Scancode> KeyMap;
  KeyMap keyMap;
  Uint8 prevKeyState[SDL_NUM_SCANCODES];

  virtual void OnKeyPressed(SDL_Scancode key) { }
  virtual void OnKeyReleased(SDL_Scancode key) { }
};
