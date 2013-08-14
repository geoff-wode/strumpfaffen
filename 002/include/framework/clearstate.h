#if ! defined(__CLEAR_STATE_H__)
#define __CLEAR_STATE_H__

#include <glm/glm.hpp>

namespace Framework
{
	struct ClearState
	{
		struct Buffers
		{
			enum Enum
			{
				Colour = 1,
				DepthStencil = 2,
				All = Colour | DepthStencil
			};
		};

		ClearState()
			: colour(0,0,1,1), colourMask(true),
				depth(1.0f), depthMask(true),
				buffers(Buffers::All)
		{
		}

		void Set(const ClearState& newState)
		{
			if (newState.colour != colour)
			{
				colour = newState.colour;
				glClearColor(colour.r, colour.g, colour.b, colour.a);
			}
			if (newState.colourMask != colourMask)
			{
				colourMask = newState.colourMask;
				glColorMask(colourMask.r, colourMask.g, colourMask.b, colourMask.a);
			}
			if (newState.depth != depth)
			{
				depth = newState.depth;
				glClearDepth(depth);
			}
			if (newState.depthMask != depthMask)
			{
				depthMask = newState.depthMask;
				glDepthMask(depthMask);
			}
		}

		void ForceDefaults()
		{
			glClearColor(colour.r, colour.g, colour.b, colour.a);
			glColorMask(colourMask.r, colourMask.g, colourMask.b, colourMask.a);
			glClearDepth(depth);
			glDepthMask(depthMask);
		}

		glm::vec4 colour;
		glm::bvec4 colourMask;

		float depth;
		bool depthMask;

		Buffers::Enum buffers;
	};
}

#endif // __CLEAR_STATE_H__
