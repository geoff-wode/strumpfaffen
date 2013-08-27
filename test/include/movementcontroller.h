#if ! defined(__MOVEMENT_CONTROLLER__)
#define __MOVEMENT_CONTROLLER__

#include <SDL.h>
#include <glm/glm.hpp>
#include <scenegraph/scenenode.h>

class MovementController
{
public:
	const glm::mat4& ToWorld;
	const glm::mat4& FromWorld;
	const glm::mat4& Position;

	MovementController(SceneNodePtr object);
	
	void Update(unsigned int elapsedMS);

	void OnKeyDown(SDL_Scancode key);
	void OnKeyUp(SDL_Scancode key);
	void OnMouseMove(const glm::ivec2& delta);
	void OnMouseLeftButtonDown(const glm::ivec2& pos);
	void OnMouseLeftButtonUp(const glm::ivec2& pos);
	void OnMouseRightButtonDown(const glm::ivec2& pos);
	void OnMouseRightButtonUp(const glm::ivec2& pos);

private:
	glm::mat4		toWorld;
	glm::mat4		fromWorld;
	glm::mat4		position;

	SceneNodePtr	object;					// object being controlled

	bool				leftMouseDown;
	glm::ivec2	mousePos;					// current mouse position
	glm::ivec2	mousePosOnDown;		// mouse position at time left button was pressed

	float				yaw;
	float				pitch;
	float				targetYaw;
	float				targetPitch;
	float				yawOnDown;				// amount of yaw at time left button was pressed
	float				pitchOnDown;			// amount of pitch at time left button was pressed

	Uint32			oldMouseButtons;
	Uint8				oldKeyState[SDL_NUM_SCANCODES];	// key state (keys[n] == true :-> pressed

	void UpdateMouse();
};

#endif // __MOVEMENT_CONTROLLER__
