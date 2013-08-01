// Defines on-screen window into which rendering is made.

#if ! defined(__VIEWPORT_H__)
#define __VIEWPORT_H__

#include <glm/glm.hpp>

namespace Framework
{

	class Viewport
	{
	public:
		Viewport();
		Viewport(int width, int height);

		float GetAspectRatio() const;

		glm::ivec2	Dimensions;
		glm::ivec2	LowerLeft;
	};

}

#endif // __VIEWPORT_H__
