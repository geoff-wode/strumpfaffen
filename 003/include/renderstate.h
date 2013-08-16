#if ! defined(__RENDER_STATE__)
#define __RENDER_STATE__

#include <glm/glm.hpp>
#include <boost/shared_ptr.hpp>
#include <buffer.h>
#include <geometry/vertexarray.h>
#include <shaders/shaderprogram.h>

struct RenderState
{
	RenderState()
		: colourMask(true),
			depthMask(true)
	{
	}

	glm::bvec4 colourMask;
	bool depthMask;

	boost::shared_ptr<ShaderProgram> shader;

	boost::shared_ptr<VertexArray> vertexArray;
};

#endif // __RENDER_STATE__
