// Defines the set of parameters used to clear buffers.

#if ! defined(__RENDER_CLEARSTATE__)
#define __RENDER_CLEARSTATE__

#include <glm/glm.hpp>

struct ClearState
{
	ClearState()
		: colour(0,0,0,0), colourMask(true, true, true, true),
			depth(1.0f), depthMask(true)
	{
	}

	glm::vec4 colour;
	glm::bvec4 colourMask;

	float depth;
	bool depthMask;

	struct Buffers
	{
		enum Enum
		{
			Colour = 1,
			Depth = 2,
			Stencil = 4,
			ColourDepth = Colour | Depth,
			DepthStencil = Depth | Stencil,
			All = Colour | Depth | Stencil
		};
	};
	Buffers::Enum buffers;
};

#endif // __RENDER_CLEARSTATE__
