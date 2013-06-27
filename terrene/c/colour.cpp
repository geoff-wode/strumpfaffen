#include <colour.h>

using namespace Terrene;

bool Colour::operator==(const Colour& other)
{
	return	(r == other.r)
			&&	(g == other.g)
			&&	(b == other.b)
			&&	(a == other.a);
}

bool Colour::operator!=(const Colour& other)
{
	return	(r != other.r)
			||	(g != other.g)
			||	(b != other.b)
			||	(a != other.a);
}

Colour::Colour()
{
}

Colour::Colour(float r, float g, float b, float a)
	: r(r), g(g), b(b), a(a)
{
}

Colour::Colour(int r, int g, int b, int a)
{
	this->r = 255.0f / r;
	this->g = 255.0f / g;
	this->b = 255.0f / b;
	this->a = 255.0f / a;
}

Colour::Colour(const Colour& other)
{
	r = other.r;
	g = other.g;
	b = other.b;
	a = other.a;
}

const Colour Colour::Black(0.0f, 0.0f, 0.0f, 1.0f);
const Colour Colour::White(1.0f, 1.0f, 1.0f, 1.0f);
const Colour Colour::Red(1.0f, 0.0f, 0.0f, 1.0f);
const Colour Colour::Green(0.0f, 1.0f, 0.0f, 1.0f);
const Colour Colour::Blue(0.0f, 0.0f, 1.0f, 1.0f);
