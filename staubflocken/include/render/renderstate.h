// Defines the set of parameters used when issuing a draw call.

#if ! defined(__RENDER_RENDERSTATE__)
#define __RENDER_RENDERSTATE__

#include <glm/glm.hpp>
#include <buffers/vertexbuffer.h>

struct RenderState
{
	RenderState()
		: colourMask(true, true, true, true),
			depthMask(true)
	{
	}

	glm::bvec4 colourMask;
	bool depthMask;

	static const size_t MaxVertexBuffers = 8;
	boost::shared_ptr<VertexBuffer> vertexBuffers[MaxVertexBuffers];
};

#endif // __RENDER_RENDERSTATE__
