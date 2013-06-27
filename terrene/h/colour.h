#if ! defined(__COLOUR_H__)
#define __COLOUR_H__

#include <glm/glm.hpp>

namespace Terrene
{

	struct Colour
	{
		float r;
		float g;
		float b;
		float a;

		Colour();
		Colour(float r, float g, float b, float a);
		Colour(int r, int g, int b, int a);
		Colour(const Colour& other);

		bool operator==(const Colour& other);
		bool operator!=(const Colour& other);

		static const Colour Black;
		static const Colour White;
		static const Colour Red;
		static const Colour Green;
		static const Colour Blue;
	};

}

#endif // __COLOUR_H__
