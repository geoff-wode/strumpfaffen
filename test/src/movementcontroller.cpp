#include <memory>
#include <movementcontroller.h>
#include <glm/ext.hpp>

//-----------------------------------------------------------------

static bool KeyDown(SDL_Scancode key, Uint8 oldState[], Uint8 newState[]);
static bool KeyHeld(SDL_Scancode key, Uint8 oldState[], Uint8 newState[]);
static bool KeyUp(SDL_Scancode key, Uint8 oldState[], Uint8 newState[]);

static bool MouseButtonDown(Uint32 button, Uint32 oldState, Uint32 newState);
static bool MouseButtonHeld(Uint32 button, Uint32 oldState, Uint32 newState);
static bool MouseButtonUp(Uint32 button, Uint32 oldState, Uint32 newState);

//-----------------------------------------------------------------

MovementController::MovementController(SceneNodePtr object)
	: ToWorld(toWorld), FromWorld(fromWorld), Position(position),
		object(object),
		leftMouseDown(false)
{
	std::memcpy(oldKeyState, SDL_GetKeyboardState(NULL), sizeof(oldKeyState));
	oldMouseButtons = SDL_GetMouseState(NULL, NULL);
}

//-----------------------------------------------------------------
void MovementController::Update(unsigned int elapsedMS)
{
	UpdateMouse();

	const Uint8* keyState = SDL_GetKeyboardState(NULL);

	if (keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_S])
	{
		// Are we moving forward or back..?
		glm::vec4 at(0,0,1,0);
		if (keyState[SDL_SCANCODE_S]) { at.z = -1; }

		// Determine the "look-at" vector...
		glm::vec4 atWorld(toWorld * at);
		atWorld *= 0.1f;

		// Modify the position...
		const glm::mat4 camTranslate(glm::translate(atWorld.x, atWorld.y, atWorld.z));
		glm::mul(position, camTranslate);

		if (leftMouseDown)
		{
			// Pitch and yaw only when the mouse button is down...
			yaw += (targetYaw - yaw) * 0.35f;
			targetPitch = glm::max(-89.9f, glm::min(89.9f, targetPitch));
			pitch += (targetPitch - pitch) * 0.35f;

			const glm::mat4 rot(glm::yawPitchRoll(yaw, pitch, 0.0f));

			toWorld = rot * position;
			fromWorld = glm::inverse(toWorld);

			object->SetTransform(toWorld, fromWorld);
		}
	}
}

//-----------------------------------------------------------------

void MovementController::UpdateMouse()
{
	glm::ivec2 mousePos;
	const Uint32 buttons = SDL_GetMouseState(&mousePos.x, &mousePos.y);

	// If button is pressed start moving the controlled object...
	if (MouseButtonDown(SDL_BUTTON_LEFT, buttons, oldMouseButtons))
	{
		leftMouseDown = true;
		mousePosOnDown = mousePos;
		yawOnDown = targetYaw;
		pitchOnDown = targetPitch;
	}
	else if (MouseButtonHeld(SDL_BUTTON_LEFT, buttons, oldMouseButtons))
	{
		targetYaw = yawOnDown + (mousePosOnDown.x - mousePos.x);
		targetPitch = pitchOnDown + (mousePos.y - mousePosOnDown.y);
	}
	else if (MouseButtonUp(SDL_BUTTON_LEFT, buttons, oldMouseButtons))
	{
		leftMouseDown = false;
	}

	oldMouseButtons = buttons;
}

//-----------------------------------------------------------------
void MovementController::UpdateKeyboard()
{
}

//-----------------------------------------------------------------

static bool KeyDown(SDL_Scancode key, Uint8 oldState[], Uint8 newState[])
{
	return (!oldState[key] && newState[key]);
}

static bool KeyHeld(SDL_Scancode key, Uint8 oldState[], Uint8 newState[])
{
	return (oldState[key] && newState[key]);
}

static bool KeyUp(SDL_Scancode key, Uint8 oldState[], Uint8 newState[])
{
	return (oldState[key] && !newState[key]);
}

//-----------------------------------------------------------------

static bool MouseButtonDown(Uint32 button, Uint32 oldState, Uint32 newState)
{
	return ((oldState & button) != button) && ((newState & button) == button);
}

static bool MouseButtonHeld(Uint32 button, Uint32 oldState, Uint32 newState)
{
	return ((oldState & button) == button) && ((newState & button) == button);
}

static bool MouseButtonUp(Uint32 button, Uint32 oldState, Uint32 newState)
{
	return ((oldState & button) == button) && ((newState & button) != button);
}
