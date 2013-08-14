#if ! defined(__RENDER_STATE__)
#define __RENDER_STATE__

#include <glm/glm.hpp>
#include <boost/shared_ptr.hpp>
#include <buffer.h>
#include <geometry/vertexdeclaration.h>
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

	ShaderProgram* shader;
	const VertexDeclaration* vertexDecl;

	boost::shared_ptr<Buffer> vertexBuffer;
};

#endif // __RENDER_STATE__
