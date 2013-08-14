#if ! defined(__MOVEMENT_CONTROLLER_H__)
#define __MOVEMENT_CONTROLLER_H__

#include <SDL.h>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <framework/input/keyboard.h>
#include <framework/input/mouse.h>
#include <framework/scenegraph/scenenode.h>

namespace Framework
{
	namespace Input
	{
		typedef void (*KeyEventFn)(SDLKey key);

		typedef boost::shared_ptr<class MovementController> MovementControllerPtr;

		class MovementController : public IMouse
		{
		public:
			MovementController();

			virtual ~MovementController();

			const glm::mat4& ToWorld() const;
			const glm::mat4& FromWorld() const;

			void SetObject(SceneNodePtr object);
			void Update(unsigned int elapsedMS);
			
			KeyEventFn OnKeyDown;
			KeyEventFn OnKeyUp;

			virtual void OnMouseMove(const glm::ivec2& pos);
			virtual void OnLeftButtonDown(const glm::ivec2& pos);
			virtual void OnLeftButtonUp(const glm::ivec2& pos);
			virtual void OnRightButtonDown(const glm::ivec2& pos);
			virtual void OnRightButtonUp(const glm::ivec2& pos);

		protected:
			SceneNodePtr object;
			
			Uint8 keys[SDLK_LAST];

			bool mouseButtonDown;
			glm::ivec2 mousePos;
			glm::ivec2 mousePosOnDown;

			glm::mat4 toWorld;
			glm::mat4 fromWorld;
			glm::mat4 position;

			float targetYaw;
			float targetPitch;
			float yaw;
			float pitch;
			float yawOnDown;
			float pitchOnDown;
		};
	}
}

#endif // __MOVEMENT_CONTROLLER_H__
