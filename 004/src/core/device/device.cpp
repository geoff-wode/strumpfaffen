#include <core/device.h>
#include <gl_loader/gl_loader.h>
#include <boost/make_shared.hpp>

using namespace core;

//------------------------------------------------------------------------

struct ShaderGlobals
{
	glm::mat4 WorldMatrix;
	glm::mat4 InverseWorldMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
	glm::mat4 ViewProjectionMatrix;
	glm::mat4 WorldViewProjectionMatrix;
	glm::vec4 CameraPos;
};

//------------------------------------------------------------------------

static void InitialiseSDL();
static void ApplyColourMask(glm::bvec4& currentMask, const glm::bvec4& newMask);
static void ApplyDepthMask(bool& currentMask, bool newMask);
static void ApplyShader(RenderState& currentState, const RenderState& newState, Device* const device);
static void ApplyVertexBuffers(RenderState& currentState, const RenderState& newState);
static void ApplyIndexBuffer(RenderState& currentState, const RenderState& newState);

//------------------------------------------------------------------------

Device::Device()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	atexit(SDL_Quit);
}

Device::~Device()
{
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(mainWindow);
}

//------------------------------------------------------------------------
bool Device::Initialise()
{
	InitialiseSDL();
	mainWindow = SDL_CreateWindow
		(
			"Strumpfaffen",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			BackbufferWidth, BackbufferHeight,
			SDL_WINDOW_OPENGL
		);
	if (NULL == mainWindow) { return false; }
	
	glContext = SDL_GL_CreateContext(mainWindow);
	if (NULL == glContext) { return false; }

	viewport.x = 0;
	viewport.y = 0;
	SDL_GetWindowSize(mainWindow, &viewport.width, &viewport.height);

	ogl_LoadFunctions();

	shaderGlobalsBuffer = boost::make_shared<UniformBuffer>(sizeof(ShaderGlobals), BufferUsage::Dynamic, 0);

	return true;
}

//------------------------------------------------------------------------

void Device::Clear(ClearBufferType::Enum buffers, const ClearState& newState)
{
	if (clearState.colour != newState.colour)
	{
		clearState.colour = newState.colour;
		glClearColor
			(
				clearState.colour.r,
				clearState.colour.g,
				clearState.colour.b,
				clearState.colour.a
			);
	}

	if (clearState.depth != newState.depth)
	{
		clearState.depth = newState.depth;
		glClearDepth(clearState.depth);
	}

	if (clearState.stencil != newState.stencil)
	{
		clearState.stencil = newState.stencil;
		glClearStencil(clearState.stencil);
	}

	ApplyColourMask(clearState.writeColour, newState.writeColour);
	ApplyDepthMask(clearState.writeDepth, newState.writeDepth);

	GLuint glBuffers = 0;
	if (buffers & ClearBufferType::Colour) { glBuffers |= GL_COLOR_BUFFER_BIT; }
	if (buffers & ClearBufferType::DepthStencil) { glBuffers |= (GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); }
	glClear(glBuffers);
}

//------------------------------------------------------------------------

void Device::DrawPrimitives(GLenum type, size_t numPrimitives, size_t startVertex, const RenderState& newState)
{
	ApplyColourMask(clearState.writeColour, newState.writeColour);
	ApplyDepthMask(clearState.writeDepth, newState.writeDepth);
	ApplyShader(renderState, newState, this);

	// ApplyVertexArrays
	// ApplyVertexBuffers
	// ApplyIndexBuffer

	int numElements = 0;
	switch (type)
	{
	case GL_TRIANGLES: numElements = numPrimitives * 3; break;
	case GL_TRIANGLE_STRIP: numElements = numPrimitives + 2; break;
	default: return;
	}

	if (renderState.indexBuffer)
	{
		glDrawElements(type, numElements, renderState.indexBuffer->DataType, (const void*)startVertex);
	}
	else
	{
		glDrawArrays(type, startVertex, numElements);
	}
}

//------------------------------------------------------------------------

void Device::SetViewport(const Viewport& viewport)
{
	this->viewport = viewport;
}

Viewport Device::GetViewport() const
{
	return viewport;
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

//------------------------------------------------------------------------
static void InitialiseSDL()
{
	const struct SDLAttribute
	{
		SDL_GLattr  attr;
		int         value;
	} sdlAttributes[] =
	{
		{ SDL_GL_DOUBLEBUFFER,				1 },
		{ SDL_GL_RED_SIZE,						8 },
		{ SDL_GL_GREEN_SIZE,					8 },
		{ SDL_GL_BLUE_SIZE,						8 },
		{ SDL_GL_ALPHA_SIZE,					8 },
		{ SDL_GL_BUFFER_SIZE,					24 },
		{ SDL_GL_DEPTH_SIZE,					24 },
		{ SDL_GL_STENCIL_SIZE,				8 },
		{ SDL_GL_MULTISAMPLEBUFFERS,	1 },
		{ SDL_GL_MULTISAMPLESAMPLES,	4 }
	};
	const int NumAttributes = sizeof(sdlAttributes)/sizeof(sdlAttributes[0]);

	for (int i = 0; i < NumAttributes; ++i)
	{
		SDL_GL_SetAttribute(sdlAttributes[i].attr, sdlAttributes[i].value);
	}
}
