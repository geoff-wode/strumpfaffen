#if ! defined(__VIEWPORT_H__)
#define __VIEWPORT_H__

namespace Terrene
{

	struct Viewport
	{
		Viewport();
		Viewport(int x, int y, int width, int height);

		int X;
		int Y;
		int Width;
		int Height;
		float AspectRatio;
	};

}

#endif // __VIEWPORT_H__
