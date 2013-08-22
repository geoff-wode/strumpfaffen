#if ! defined(__CORE_CLEARSTATE__)
#define __CORE_CLEARSTATE__

#include <glm/glm.hpp>

namespace core
{
	struct ClearState
	{
		ClearState()
			: colour(0), depth(1.0f), stencil(0), writeDepth(true), writeColour(true)
		{ }

		// The value to which the colour buffer will be cleared.
		glm::vec4 colour;

		// The value to which the depth buffer will be cleared.
		float depth;

		// The value to which the stencil buffer will be cleared.
		unsigned int stencil;

		// Whether to write to the depth buffer or not.
		bool writeDepth;

		// Whether to write to any of the RGBA colour channels or not.
		glm::bvec4 writeColour;
	};

	namespace ClearBufferType
	{
		enum Enum
		{
			Colour = 1,
			DepthStencil = 2,
			All = Colour | DepthStencil
		};
	}
}

#endif // __CORE_CLEARSTATE__
