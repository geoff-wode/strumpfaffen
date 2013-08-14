#if ! defined(__INPUT_MOUSE_H__)
#define __INPUT_MOUSE_H__

#include <glm/glm.hpp>

namespace Framework
{
	namespace Input
	{
		class IMouse
		{
		public:
			virtual void OnMouseMove(const glm::ivec2& pos) = 0;
			virtual void OnLeftButtonDown(const glm::ivec2& pos) = 0;
			virtual void OnLeftButtonUp(const glm::ivec2& pos) = 0;
			virtual void OnRightButtonDown(const glm::ivec2& pos) = 0;
			virtual void OnRightButtonUp(const glm::ivec2& pos) = 0;
		};
	}
}

#endif // __INPUT_MOUSE_H__
