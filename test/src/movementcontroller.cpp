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
void MovementController::OnKeyDown(SDL_Scancode key)
{
}
//-----------------------------------------------------------------
void MovementController::OnKeyUp(SDL_Scancode key)
{
}
//-----------------------------------------------------------------
void MovementController::OnMouseMove(const glm::ivec2& delta)
{
}
//-----------------------------------------------------------------
void MovementController::OnMouseLeftButtonDown(const glm::ivec2& pos)
{
}
//-----------------------------------------------------------------
void MovementController::OnMouseLeftButtonUp(const glm::ivec2& pos)
{
}
//-----------------------------------------------------------------
void MovementController::OnMouseRightButtonDown(const glm::ivec2& pos)
{
}
//-----------------------------------------------------------------
void MovementController::OnMouseRightButtonUp(const glm::ivec2& pos)
{
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
	glm::ivec2 newPos;
	const Uint32 buttons = SDL_GetMouseState(&newPos.x, &newPos.y);

	// If button is pressed start moving the controlled object...
	if (MouseButtonDown(SDL_BUTTON_LEFT, oldMouseButtons, buttons))
	{
		leftMouseDown = true;
		mousePosOnDown = mousePos;
		yawOnDown = targetYaw;
		pitchOnDown = targetPitch;
	}
	else if (MouseButtonHeld(SDL_BUTTON_LEFT, oldMouseButtons, buttons))
	{
		const glm::ivec2 deltaPos(newPos - mousePos);
		targetYaw = yawOnDown + (mousePosOnDown.x - deltaPos.x);
		targetPitch = pitchOnDown + (deltaPos.y - mousePosOnDown.y);
	}
	else if (MouseButtonUp(SDL_BUTTON_LEFT, oldMouseButtons, buttons))
	{
		leftMouseDown = false;
	}

	oldMouseButtons = buttons;
	mousePos = newPos;
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
	return !(oldState & SDL_BUTTON(button)) && (newState & SDL_BUTTON(button));
}

static bool MouseButtonHeld(Uint32 button, Uint32 oldState, Uint32 newState)
{
	return (oldState & SDL_BUTTON(button)) && (newState & SDL_BUTTON(button));
}

static bool MouseButtonUp(Uint32 button, Uint32 oldState, Uint32 newState)
{
	return (oldState & SDL_BUTTON(button)) && !(newState & SDL_BUTTON(button));
}
