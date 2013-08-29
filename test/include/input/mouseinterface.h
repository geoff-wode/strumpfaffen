#if ! defined(__MOUSE_INTERFACE__)
#define __MOUSE_INTERFACE__

#include <glm/glm.hpp>

// Declares an interface which objects that want to get mouse events
// must implement.
class IMouse
{
public:
	virtual void OnMouseMove(const glm::ivec2& pos) = 0;
	virtual void OnMouseLeftButtonDown(const glm::ivec2& pos) = 0;
	virtual void OnMouseLeftButtonUp(const glm::ivec2& pos) = 0;
	virtual void OnMouseRightButtonDown(const glm::ivec2& pos) = 0;
	virtual void OnMouseRightButtonUp(const glm::ivec2& pos) = 0;
};

#endif // __MOUSE_INTERFACE__
