#if ! defined(__CLEAR_STATE_H__)
#define __CLEAR_STATE_H__

#include <terrene/h/colour.h>

namespace Terrene
{

	namespace ClearBuffers
	{
		enum Enum
		{
			None		= 0,
			Colour	= 1 << 0,
			Depth		= 1 << 1,
			Stencil	= 1 << 2,
			ColourDepth = Colour | Depth,
			DepthStencil = Depth | Stencil,
			All = Colour | Depth | Stencil
		};
	}

	struct ClearState
	{
		ClearState()
			: Buffers(ClearBuffers::ColourDepth),
				Depth(1),
				ClearColour(Colour::White)
		{
		}

		ClearBuffers::Enum Buffers;
		float Depth;
		Colour ClearColour;
	};

}

#endif //__CLEAR_STATE_H__
