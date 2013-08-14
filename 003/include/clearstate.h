#if ! defined(__CLEAR_STATE__)
#define __CLEAR_STATE__

#include <glm/glm.hpp>
#include <gl_loader/gl_loader.h>

namespace BufferName
{
	enum Enum
	{
		Colour = GL_COLOR_BUFFER_BIT,
		Depth = GL_DEPTH_BUFFER_BIT,
		Stencil = GL_STENCIL_BUFFER_BIT,
		DepthStencil = Depth | Stencil,
		All = Colour | DepthStencil
	};
}

// Declares the various state attributes which can be applied when clearing
// the current render target.
struct ClearState
{
	ClearState()
		: colour(0,0,1,1), colourMask(true),
			depth(1.0f), depthMask(true),
			buffers(BufferName::All)
	{
	}

	glm::vec4 colour;
	glm::bvec4 colourMask;

	float depth;
	bool depthMask;

	BufferName::Enum buffers;
};

#endif // __CLEAR_STATE__
