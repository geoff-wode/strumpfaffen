#include <core/device.h>
#include <core/uniformbuffer.h>
#include <gl_loader/gl_loader.h>

using namespace core;

//------------------------------------------------------------------------

typedef std::map<std::string, boost::shared_ptr<Shader>> ShaderCache;

struct Device::Impl
{
	// The implementation creates a uniform buffer for the shader globals and binds it to a
	// known, fixed binding point (zero) which shaders can rely on to bind their uniform block
	// to.
	Impl()
		: uniformBuffer(new UniformBuffer(sizeof(Impl::ShaderGlobals), BufferUsage::Dynamic, 0))
	{
		glClearColor(clearState.colour.r, clearState.colour.g, clearState.colour.b, clearState.colour.a);
		glClearDepth(clearState.depth);
		glClearStencil(clearState.stencil);
		glColorMask(clearState.writeColour.r, clearState.writeColour.g, clearState.writeColour.b, clearState.writeColour.a);
		glDepthMask(clearState.writeDepth);
	}

	void SetSharedShaderUniforms(const Device::ShaderGlobals& deviceGlobals)
	{
		// Copy the public values into the private buffer, computing useful derived values along the way
		// (e.g. the concatenated World-View-Projection matrix...

		globals.WorldMatrix = deviceGlobals.WorldMatrix;
		globals.ViewMatrix = deviceGlobals.ViewMatrix;
		globals.ProjectionMatrix = deviceGlobals.ProjectionMatrix;
		globals.ViewProjectionMatrix = globals.ProjectionMatrix * globals.ViewMatrix;
		globals.WorldViewProjectionMatrix = globals.ViewProjectionMatrix * globals.WorldMatrix;
		globals.CameraPosition = glm::vec4(deviceGlobals.CameraPosition, 1.0f);

		uniformBuffer->Activate();
		uniformBuffer->Set(&globals, sizeof(globals), 0);
		uniformBuffer->Deactivate();
	}

	RenderState renderState;
	ClearState clearState;

	boost::scoped_ptr<UniformBuffer> uniformBuffer;

	ShaderCache shaderCache;

	// This is the data that is passed to the uniform buffer shared by all the shaders.
	// NOTE: It is >vital< to keep these in the same order as the uniform block defined
	// in core\shadercompilerdefines.h, otherwise Bad Things(tm) will happen.
	struct ShaderGlobals
	{
		glm::mat4 WorldMatrix;
		glm::mat4 ViewMatrix;
		glm::mat4 ProjectionMatrix;
		glm::mat4 ViewProjectionMatrix;
		glm::mat4 WorldViewProjectionMatrix;
		glm::vec4 CameraPosition;
	};
	ShaderGlobals globals;
};

//------------------------------------------------------------------------

static void ApplyColourMask(glm::bvec4& currentMask, const glm::bvec4& newMask);
static void ApplyDepthMask(bool& currentMask, bool newMask);
static void ApplyShader(RenderState& currentState, const RenderState& newState, Device* const device);
static void ApplyVertexBuffers(RenderState& currentState, const RenderState& newState);
static void ApplyIndexBuffer(RenderState& currentState, const RenderState& newState);

//------------------------------------------------------------------------

Device::Device()
	: impl(new Impl())
{
}

Device::~Device()
{
}

//------------------------------------------------------------------------

void Device::Clear(ClearBufferType::Enum buffers, const ClearState& clearState)
{
	if (impl->clearState.colour != clearState.colour)
	{
		impl->clearState.colour = clearState.colour;
		glClearColor
			(
				impl->clearState.colour.r,
				impl->clearState.colour.g,
				impl->clearState.colour.b,
				impl->clearState.colour.a
			);
	}

	if (impl->clearState.depth != clearState.depth)
	{
		impl->clearState.depth = clearState.depth;
		glClearDepth(impl->clearState.depth);
	}

	if (impl->clearState.stencil != clearState.stencil)
	{
		impl->clearState.stencil = clearState.stencil;
		glClearStencil(impl->clearState.stencil);
	}

	ApplyColourMask(impl->clearState.writeColour, clearState.writeColour);
	ApplyDepthMask(impl->clearState.writeDepth, clearState.writeDepth);

	GLuint glBuffers = 0;
	if (buffers & ClearBufferType::Colour) { glBuffers |= GL_COLOR_BUFFER_BIT; }
	if (buffers & ClearBufferType::DepthStencil) { glBuffers |= (GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); }
	glClear(glBuffers);
}

//------------------------------------------------------------------------

void Device::DrawPrimitives(
			PrimitiveType::Enum type,
			size_t numPrimitives,
			size_t startVertex,
			const RenderState& state)
{
	impl->SetSharedShaderUniforms(shaderGlobals);

	ApplyColourMask(impl->clearState.writeColour, state.writeColour);
	ApplyDepthMask(impl->clearState.writeDepth, state.writeDepth);
	ApplyShader(impl->renderState, state, this);
}

//------------------------------------------------------------------------

boost::shared_ptr<Shader> Device::CreateShader(const std::string& name)
{
	ShaderCache::const_iterator iter = impl->shaderCache.find(name);
	if (impl->shaderCache.cend() == iter)
	{
		boost::shared_ptr<Shader> shader(new Shader(name));
		impl->shaderCache[name] = shader;
		return shader;
	}
	return iter->second;
}

//------------------------------------------------------------------------

static void ApplyColourMask(glm::bvec4& currentMask, const glm::bvec4& newMask)
{
	if (currentMask != newMask)
	{
		currentMask = newMask;
		glColorMask
			(
				currentMask.r,
				currentMask.g,
				currentMask.b,
				currentMask.a
			);
	}
}

//------------------------------------------------------------------------
static void ApplyDepthMask(bool& currentMask, bool newMask)
{
	if (currentMask != newMask)
	{
		currentMask = newMask;
		glDepthMask(currentMask);
	}
}

//------------------------------------------------------------------------
static void ApplyShader(RenderState& currentState, const RenderState& newState, Device* const device)
{
	if (currentState.shader != newState.shader)
	{
		currentState.shader = newState.shader;
		currentState.shader->Activate();
	}

	currentState.shader->Update();
}

//------------------------------------------------------------------------
static void ApplyVertexBuffers(RenderState& currentState, const RenderState& newState)
{
	for (size_t buffer = 0; buffer < RenderState::MaxVertexBuffers; ++buffer)
	{
		if (currentState.vertexBuffers[buffer] != newState.vertexBuffers[buffer])
		{
			if (newState.vertexBuffers[buffer])
			{
				currentState.vertexBuffers[buffer] = newState.vertexBuffers[buffer];
				currentState.vertexBuffers[buffer]->Activate();
			}
			else
			{
				currentState.vertexBuffers[buffer]->Deactivate();
				currentState.vertexBuffers[buffer] = NULL;
			}
		}
	}
}

//------------------------------------------------------------------------
static void ApplyIndexBuffer(RenderState& currentState, const RenderState& newState)
{
	if (currentState.indexBuffer != newState.indexBuffer)
	{
		if (!newState.indexBuffer)
		{
			IndexBuffer::Deactivate();
			currentState.indexBuffer = NULL;
		}
		else
		{
			currentState.indexBuffer = newState.indexBuffer;
			currentState.indexBuffer->Activate();
		}
	}
}
