#if ! defined(__CORE_RENDERSTATE__)
#define __CORE_RENDERSTATE__

#include <glm/glm.hpp>
#include <boost/shared_ptr.hpp>
#include <core/vertexbuffer.h>
#include <core/indexbuffer.h>
#include <core/shaders/shader.h>

namespace core
{
	struct RenderState
	{
		RenderState() : writeDepth(true), writeColour(true) { }

		// Whether to write to the depth buffer or not.
		bool writeDepth;

		// Whether to write to any of the RGBA colour channels or not.
		glm::bvec4 writeColour;

		// The shader to use when rendering.
		boost::shared_ptr<Shader> shader;

		// Maximum number of vertex buffers which can pass data to the shader.
		static const size_t MaxVertexBuffers = 16;

		// A set of vertex buffers carrying vertex attribute data to the shader.
		boost::shared_ptr<VertexBuffer> vertexBuffers[MaxVertexBuffers];

		// Index buffer carrying vertex indices for indexed primitive rendering.
		// If no index buffer is specified, non-indexed rendering is used.
		boost::shared_ptr<IndexBuffer> indexBuffer;
	};
}

#endif // __CORE_RENDERSTATE__
