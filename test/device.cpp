
#include <string>
#include <map>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <gl_loader/gl_loader.h>

#include "device.h"
#include "buffers.h"
#include "shadersemantic.h"
#include "vertexelement.h"
#include "shader.h"

//--------------------------------------------------------------------------------------------

std::map<std::string, GLuint> Device::uniformBlockIndices;

//--------------------------------------------------------------------------------------------

Device::Device()
{
	uniformBuffer = boost::make_shared<UniformBufferClass>(sizeof(CommonShaderVarsBlock), 0);
	uniformBlockIndices["CommonShaderVarsBlock"] = 0;

	glColorMask(renderState.colourMask.r, renderState.colourMask.g, renderState.colourMask.b, renderState.colourMask.a);
	glDepthMask(renderState.depthMask);
	glPolygonMode(GL_FRONT_AND_BACK, renderState.polygonMode);
}

Device::~Device()
{
}

//--------------------------------------------------------------------------------------------

void Device::Draw(GLenum primitiveType, size_t start, size_t count, const RenderState& renderState)
{
	ApplyUniformBuffer();

	// Generic fixed-function render state...
	ApplyColourMask(renderState.colourMask);
	ApplyDepthMask(renderState.depthMask);
	ApplyPolygonMode(renderState.polygonMode);

	ApplyShader(renderState.shader);

	// The order of these is important...!
	{
		ApplyVertexBuffer(renderState.vertexBuffer);
		ApplyIndexBuffer(renderState.indexBuffer);
		ApplyVertexLayout(renderState.vertexLayout);
	}

	if (renderState.indexBuffer)
	{
		glDrawElements(primitiveType, count, renderState.indexBuffer->indexType, (const void*)start);
	}
	else
	{
		glDrawArrays(primitiveType, start, count);
	}
}

//--------------------------------------------------------------------------------------------

void Device::ApplyUniformBuffer()
{
	bool update = false;

	if (commonShaderVars.WorldMatrix != WorldMatrix)
	{
		commonShaderVars.WorldMatrix = WorldMatrix;
		commonShaderVars.InverseWorldMatrix	= glm::inverse(commonShaderVars.WorldMatrix);
		update = true;
	}

	if (commonShaderVars.ViewMatrix != ViewMatrix)
	{
		commonShaderVars.ViewMatrix					= ViewMatrix;
		commonShaderVars.InverseViewMatrix	= glm::inverse(ViewMatrix);
		update = true;
	}

	if (commonShaderVars.ProjectionMatrix != ProjectionMatrix)
	{
		commonShaderVars.ProjectionMatrix	= ProjectionMatrix;
		update = true;
	}

	if (update)
	{
		commonShaderVars.WorldViewMatrix						= commonShaderVars.ViewMatrix * commonShaderVars.WorldMatrix;
		commonShaderVars.ViewProjectionMatrix				= commonShaderVars.ProjectionMatrix * commonShaderVars.ViewMatrix;
		commonShaderVars.WorldViewProjectionMatrix	= commonShaderVars.ViewProjectionMatrix * commonShaderVars.WorldMatrix;

		uniformBuffer->SetData(&commonShaderVars, sizeof(commonShaderVars), 0);
		uniformBuffer->Enable();
	}
}

//--------------------------------------------------------------------------------------------
void Device:: ApplyShader(Shader value)
{
	if (renderState.shader != value)
	{
		renderState.shader = value;
		renderState.shader->Enable();
	}
	renderState.shader->ApplyUniforms();
}

//--------------------------------------------------------------------------------------------
void Device:: ApplyIndexBuffer(IndexBuffer value)
{
	if (renderState.indexBuffer != value)
	{
		if (value)
		{
			renderState.indexBuffer = value;
			renderState.indexBuffer->Enable();
		}
		else
		{
			renderState.indexBuffer = value;
			IndexBufferClass::Disable();
		}
	}
}
//--------------------------------------------------------------------------------------------
void Device:: ApplyVertexBuffer(VertexBuffer value)
{
	if (renderState.vertexBuffer != value)
	{
		renderState.vertexBuffer = value;
		renderState.vertexBuffer->Enable();
	}
}
//--------------------------------------------------------------------------------------------
void Device:: ApplyVertexLayout(const VertexLayout* value)
{
	if (renderState.vertexLayout != value)
	{
		// Disable any previous vertex layout attributes...
		if (renderState.vertexLayout)
		{
			for (size_t e = 0; renderState.vertexLayout->elementCount; ++e)
			{
				const ShaderSemantic* const attribute = ShaderSemantic::Get(value->elements[e].semantic);
				glDisableVertexAttribArray(attribute->attribIndex);
			}
		}

		// Enable the attributes in the new vertex layout...
		renderState.vertexLayout = value;
		for (size_t e = 0; e < value->elementCount; ++e)
		{
			const ShaderSemantic* const attribute = ShaderSemantic::Get(value->elements[e].semantic);
			if (NULL != attribute)
			{
				glEnableVertexAttribArray(attribute->attribIndex);
				glVertexAttribPointer(
					attribute->attribIndex,
					attribute->elementCount,
					attribute->elementType,
					GL_FALSE,
					value->stride,
					(const void*)value->elements[e].offset);
			}
		}
	}
}
//--------------------------------------------------------------------------------------------
void Device:: ApplyColourMask(const glm::bvec4& value)
{
	if (renderState.colourMask != value)
	{
		renderState.colourMask = value;
		glColorMask(renderState.colourMask.r, renderState.colourMask.g, renderState.colourMask.b, renderState.colourMask.a);
	}
}
//--------------------------------------------------------------------------------------------
void Device:: ApplyDepthMask(bool value)
{
	if (renderState.depthMask != value)
	{
		renderState.depthMask = value;
		glDepthMask(renderState.depthMask);
	}
}
//--------------------------------------------------------------------------------------------
void Device::ApplyPolygonMode(GLenum value)
{
	if (renderState.polygonMode != value)
	{
		renderState.polygonMode = value;
		glPolygonMode(GL_FRONT_AND_BACK, renderState.polygonMode);
	}
}