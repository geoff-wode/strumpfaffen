#include <viewport.h>

bool operator==(const Viewport& a, const Viewport& b)
{
	return (a.topLeft == b.topLeft) && (a.size == b.size);
}

bool operator!=(const Viewport& a, const Viewport& b)
{
	return (a.topLeft != b.topLeft) || (a.size != b.size);
}
