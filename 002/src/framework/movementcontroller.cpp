#include <memory>
#include <framework/input/movementcontroller.h>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

using namespace Framework;
using namespace Framework::Input;

//-------------------------------------------------------------------------

#define MIN(a,b)	((a) < (b) ? (a) : (b))
#define MAX(a,b)	((a) > (b) ? (a) : (b))

//-------------------------------------------------------------------------

MovementController::MovementController()
	: mouseButtonDown(false),
		mousePos(0), mousePosOnDown(0),
		toWorld(1), fromWorld(1), position(1),
		targetYaw(0), targetPitch(0),
		yaw(0), pitch(0),
		yawOnDown(0), pitchOnDown(0)
{
	std::memset(keys, 0, sizeof(keys));
}

MovementController::~MovementController()
{
}

const glm::mat4& MovementController::ToWorld() const
{
	return toWorld;
}

const glm::mat4& MovementController::FromWorld() const
{
	return fromWorld;
}

void MovementController::SetObject(SceneNodePtr object)
{
	this->object = object;
}

void MovementController::Update(unsigned int elapsedMS)
{
	Uint8 oldKeys[SDLK_LAST];
	std::memcpy(oldKeys, keys, sizeof(keys));
	std::memcpy(keys, SDL_GetKeyState(NULL), SDLK_LAST);

	for (size_t i = 0; i < SDLK_LAST; ++i)
	{
		if (OnKeyDown)
		{
			if (!oldKeys[i] && keys[i]) { OnKeyDown((SDLKey)i); }
		}

		if (OnKeyUp)
		{
			if (oldKeys[i] && !keys[i]) { OnKeyUp((SDLKey)i); }
		}
	}

	if (keys[SDLK_w] || keys[SDLK_s])
	{
		glm::vec4 at(0,0,1,0);
		if (keys[SDLK_s]) { at *= -1; }

		const glm::vec4 atWorld(0.1f * toWorld * at);

		const glm::mat4 camTranslate(glm::translate(atWorld.x, atWorld.y, atWorld.z));

		position *= camTranslate;

		if (mouseButtonDown)
		{
			yaw += (targetYaw - yaw) * 0.35f;
			targetPitch = MAX(-90.0f, MIN(90.0f, targetPitch));
			pitch += (targetPitch - pitch) * 0.35f;

			const glm::mat4 rot = glm::eulerAngleXY(pitch, yaw);

			toWorld = rot * position;
			fromWorld = glm::inverse(toWorld);

			if (object) { object->SetTransform(toWorld, fromWorld); }
		}
	}
}

//-------------------------------------------------------------------------

void MovementController::OnMouseMove(const glm::ivec2& pos)
{
	if (mouseButtonDown)
	{
		targetYaw = yawOnDown + (mousePosOnDown.x - mousePos.x);
		targetPitch = pitchOnDown + (mousePos.y - mousePosOnDown.y);
	}
}

void MovementController::OnLeftButtonDown(const glm::ivec2& pos)
{
	mouseButtonDown = true;
	mousePosOnDown = pos;
	yawOnDown = targetYaw;
	pitchOnDown = targetPitch;
}

void MovementController::OnLeftButtonUp(const glm::ivec2& pos)
{
	mouseButtonDown = false;
}

void MovementController::OnRightButtonDown(const glm::ivec2& pos)
{
}

void MovementController::OnRightButtonUp(const glm::ivec2& pos)
{
}
