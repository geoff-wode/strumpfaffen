#if ! defined(__RENDER_STATE__)
#define __RENDER_STATE__

#include <glm/glm.hpp>
#include <gl_loader/gl_loader.h>
#include "buffers.h"

class RenderState
{
public:
	RenderState()
		: colourMask(true), depthMask(true),
			polygonMode(GL_FILL),
			vertexLayout(NULL)
	{
	}

	glm::bvec4		colourMask;
	bool					depthMask;
	GLenum				polygonMode;
	const VertexLayout* vertexLayout;
	Shader				shader;
	VertexBuffer	vertexBuffer;
	IndexBuffer		indexBuffer;
};

#endif // __RENDER_STATE__
