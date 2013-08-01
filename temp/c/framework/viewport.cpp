#include <framework/viewport.h>

using namespace Framework;

//----------------------------------------------------------------------------------

Viewport::Viewport()
	: Dimensions(800, 600), LowerLeft(0)
{
}

Viewport::Viewport(int width, int height)
	: Dimensions(width, height), LowerLeft(0)
{
}

float Viewport::GetAspectRatio() const
{
	return (float)Dimensions.x / (float)Dimensions.y;
}
