#if ! defined(__VIEWPORT__)
#define __VIEWPORT__

#include <glm/glm.hpp>

struct Viewport
{
	glm::ivec2 topLeft;
	glm::ivec2 size;
};

bool operator==(const Viewport& a, const Viewport& b);
bool operator!=(const Viewport& a, const Viewport& b);

#endif // __VIEWPORT__
