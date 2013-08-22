#if ! defined(__CORE_VIEWPORT__)
#define __CORE_VIEWPORT__

namespace core
{
	struct Viewport
	{
		int x;
		int y;
		int width;
		int height;
		float AspectRatio() const { return (float)width / (float)height; }
	};
}

#endif // __CORE_VIEWPORT__
