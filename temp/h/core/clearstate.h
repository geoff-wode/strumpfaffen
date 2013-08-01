#if ! defined(__CLEAR_STATE_H__)
#define __CLEAR_STATE_H__

#include <glm/glm.hpp>

namespace Core
{

	namespace ClearBuffers
	{
		enum Enum
		{
			Colour = 1,
			DepthStencil = 2,
			All = Colour | DepthStencil
		};
	}

	struct ClearState
	{
		ClearState()
			: Colour(1.0f), ColourMask(true),
				Depth(1.0f), DepthMask(true),
				Buffers(ClearBuffers::All)
		{
		}

		glm::vec4	Colour;				// value(s) to clear colour channels to
		glm::bvec4 ColourMask;	// select which colour channels are cleared

		float Depth;						// value to clear depth buffer to
		bool	DepthMask;				// if true, depth is cleared

		ClearBuffers::Enum Buffers;	// which buffers to clear
	};

}
#endif // __CLEAR_STATE_H__
