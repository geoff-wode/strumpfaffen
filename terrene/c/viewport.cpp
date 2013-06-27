#include <SDL.h>
#include <gl_loader/gl_loader.h>
#include <viewport.h>
#include <debug.h>

using namespace Terrene;

Viewport::Viewport()
	: X(0), Y(0), Width(0), Height(0), AspectRatio(0.0f)
{
}

Viewport::Viewport(int x, int y, int width, int height)
	: X(x), Y(x), Width(width), Height(height)
{
	AspectRatio = (float)Width / (float)Height;
}
