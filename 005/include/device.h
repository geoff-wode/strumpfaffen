#if ! defined(__DEVICE__)
#define __DEVICE__

#include <glm/glm.hpp>
#include <gl_loader/gl_loader.h>
#include <shader.h>
#include <buffers.h>
#include <vertexarray.h>

struct ClearState
{
	ClearState()
		: colour(0), colourMask(true),
			depth(1.0f), depthMask(true)
	{
	}

	glm::vec4 colour;
	glm::bvec4 colourMask;

	float depth;
	bool depthMask;
};

struct RenderState
{
	RenderState()
		: colourMask(true), depthMask(true)
	{
	}

	glm::bvec4 colourMask;
	bool depthMask;

	boost::shared_ptr<Shader> shader;
	boost::shared_ptr<IndexBuffer> indexBuffer;
	boost::shared_ptr<VertexArray> vertexArray;
};

namespace Device
{
	bool Initialise();
	
	void SetWorldMatrix(const glm::mat4 value);
	void SetViewMatrix(const glm::mat4& value);
	void SetProjectionMatrix(const glm::mat4& value);

	void Clear(const ClearState& clearState, GLenum buffers);
	void Draw(GLenum primitiveType, size_t primitiveCount, size_t startVertex, const RenderState& renderState);
	void SwapBuffers();
};

#endif // __DEVICE__
