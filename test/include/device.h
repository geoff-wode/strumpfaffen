#if ! defined(__DEVICE__)
#define __DEVICE__

#include <string>
#include <map>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <gl_loader/gl_loader.h>

#include "buffers.h"
#include "shadersemantic.h"
#include "vertexlayout.h"
#include "shader.h"
#include "renderstate.h"

//--------------------------------------------------------------------------------------------
class Device
{
public:
	glm::vec4 CameraPos;
	glm::mat4 WorldMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

	static std::map<std::string, GLuint> uniformBlockIndices;

	Device();
	~Device();

	// Configure the render state and draw.
	void Draw(GLenum primitiveType, size_t start, size_t count, const RenderState& renderState);

private:
	RenderState		renderState;

	UniformBuffer	uniformBuffer;
	struct CommonShaderVarsBlock
	{
		glm::vec4 CameraPos;
		glm::mat4 WorldMatrix;
		glm::mat4 ViewMatrix;
		glm::mat4 ProjectionMatrix;
		glm::mat4 InverseWorldMatrix;
		glm::mat4 InverseViewMatrix;
		glm::mat4 WorldViewMatrix;
		glm::mat4 ViewProjectionMatrix;
		glm::mat4 WorldViewProjectionMatrix;
	} commonShaderVars;

	void ApplyUniformBuffer();
	void ApplyShader(Shader value);
	void ApplyIndexBuffer(IndexBuffer value);
	void ApplyVertexBuffer(VertexBuffer value);
	void ApplyVertexLayout(const VertexLayout* value);
	void ApplyColourMask(const glm::bvec4& value);
	void ApplyDepthMask(bool value);
	void ApplyPolygonMode(GLenum value);
};

#endif // __DEVICE__
