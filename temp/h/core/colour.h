// Defines a type for storing colours as well as some pre-defined colours.

#if ! defined(__COLOUR_H__)
#define __COLOUR_H__

#include <glm/glm.hpp>

namespace Core
{
	typedef glm::vec4 Colour;

	static const Colour Black(0,0,0,0);
	static const Colour White(1,1,1,1);
	static const Colour Red(1,0,0,1);
	static const Colour Green(0,1,0,1);
	static const Colour Blue(0,0,1,1);
}

#endif // __COLOUR_H__
