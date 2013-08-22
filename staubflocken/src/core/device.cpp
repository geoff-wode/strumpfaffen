#include <SDL.h>
#include <stdlib.h>
#include <core/device.h>
#include <core/debug.h>
#include <gl_loader/gl_loader.h>

using namespace core;

//---------------------------------------------------------------------

struct Device::Impl
{
	std::string programTitle;
	int backbufferWidth;
	int backbufferHeight;
	SDL_Window*		mainWindow;
	SDL_GLContext	glContext;
	ClearState clearState;
	RenderState renderState;
};

//---------------------------------------------------------------------

static void ConfigureGLContext();
static void InitialiseGLState(ClearState& clearState, RenderState& renderState);
static void ApplyClearColour(glm::vec4& current, const glm::vec4& input);
static void ApplyColourMask	(glm::bvec4& current, const glm::bvec4& input);
static void ApplyClearDepth	(float& current, float input);
static void ApplyDepthMask	(bool& current, bool input);
static void ApplyVertexBuffers(boost::shared_ptr<VertexBuffer> current[], const boost::shared_ptr<VertexBuffer> input[]);

//---------------------------------------------------------------------

Device::Device()
	: impl(new Impl())
{
}

Device::~Device()
{
	SDL_GL_DeleteContext(impl->glContext);
	SDL_DestroyWindow(impl->mainWindow);
}

//---------------------------------------------------------------------

bool Device::Initialise()
{
	// Configure low-level stuff (bpp, msaa, etc.)...
	ConfigureGLContext();

	impl->mainWindow = SDL_CreateWindow(
		WindowTitle.c_str(),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		BackbufferWidth, BackbufferHeight,
		SDL_WINDOW_OPENGL);
	if (!impl->mainWindow)
	{
		LOG("Device::Initialise - failed:\n%s\n", SDL_GetError());
		return false;
	}

	impl->glContext = SDL_GL_CreateContext(impl->mainWindow);
	if (!impl->glContext)
	{
		LOG("Device::Initialise - failed:\n%s\n", SDL_GetError());
		return false;
	}

	// Load the OpeGL functions...
	ogl_LoadFunctions();
  LOG("OpenGL v%d.%d\n", ogl_GetMajorVersion(), ogl_GetMinorVersion());
  LOG("GLSL v%s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	// Initialise the GL state so we know what changes from call to call...
	InitialiseGLState(impl->clearState, impl->renderState);

	return true;
}

//---------------------------------------------------------------------

void Device::SwapBuffers() { SDL_GL_SwapWindow(impl->mainWindow); }

//---------------------------------------------------------------------
void Device::Clear(const ClearState& clearState, ClearState::Buffers::Enum buffers)
{
	ApplyClearColour(impl->clearState.colour, clearState.colour);
	ApplyClearDepth(impl->clearState.depth, clearState.depth);
	ApplyColourMask(impl->renderState.colourMask, clearState.colourMask);
	ApplyDepthMask(impl->renderState.depthMask, clearState.depthMask);

	GLenum bitField = 0;
	if (buffers & ClearState::Buffers::Colour) { bitField |= GL_COLOR_BUFFER_BIT; }
	if (buffers & ClearState::Buffers::Depth) { bitField |= GL_DEPTH_BUFFER_BIT; }
	if (buffers & ClearState::Buffers::Stencil) { bitField |= GL_STENCIL_BUFFER_BIT; }
	glClear(bitField);
}

//---------------------------------------------------------------------
void Device::Draw
	(
		const RenderState& renderState
	)
{
	ApplyColourMask(impl->renderState.colourMask, renderState.colourMask);
	ApplyDepthMask(impl->renderState.depthMask, renderState.depthMask);
	ApplyVertexBuffers(impl->renderState.vertexBuffers, renderState.vertexBuffers);
}

//---------------------------------------------------------------------
static void ConfigureGLContext()
{
  static const struct SDLAttribute
  {
    const char* name;
    SDL_GLattr  attr;
    int         value;
  } sdlAttributes[] =
  {
    { "SDL_GL_DOUBLEBUFFER",  SDL_GL_DOUBLEBUFFER,            1 },
    { "SDL_GL_RED_SIZE",      SDL_GL_RED_SIZE,                8 },
    { "SDL_GL_GREEN_SIZE",    SDL_GL_GREEN_SIZE,              8 },
    { "SDL_GL_BLUE_SIZE",     SDL_GL_BLUE_SIZE,               8 },
    { "SDL_GL_ALPHA_SIZE",    SDL_GL_ALPHA_SIZE,              8 },
    { "SDL_GL_BUFFER_SIZE",   SDL_GL_BUFFER_SIZE,             24 },
    { "SDL_GL_MULTISAMPLEBUFFERS", SDL_GL_MULTISAMPLEBUFFERS, 1 },
    { "SDL_GL_MULTISAMPLESAMPLES", SDL_GL_MULTISAMPLESAMPLES, 4 }
  };
  const int NumAttributes = sizeof(sdlAttributes)/sizeof(sdlAttributes[0]);
  for (int i = 0; i < NumAttributes; ++i)
  {
    SDL_GL_SetAttribute(sdlAttributes[i].attr, sdlAttributes[i].value);
  }
}

//---------------------------------------------------------------------
static void InitialiseGLState(ClearState& clearState, RenderState& renderState)
{
	glClearColor(clearState.colour.r, clearState.colour.g, clearState.colour.b, clearState.colour.a);
	glClearDepth(clearState.depth);

	glColorMask(renderState.colourMask.r, renderState.colourMask.g, renderState.colourMask.b, renderState.colourMask.a);
	glDepthMask(renderState.depthMask);
}

//---------------------------------------------------------------------
static void ApplyClearColour(glm::vec4& current, const glm::vec4& input)
{
	if (current != input)
	{
		current = input;
		glClearColor(current.r, current.g, current.b, current.a);
	}
}

//---------------------------------------------------------------------
static void ApplyColourMask	(glm::bvec4& current, const glm::bvec4& input)
{
	if (current != input)
	{
		current = input;
		glColorMask(current.r, current.g, current.b, current.a);
	}
}

//---------------------------------------------------------------------
static void ApplyClearDepth	(float& current, float input)
{
	if (current != input)
	{
		current = input;
		glClearDepth(current);
	}
}

//---------------------------------------------------------------------
static void ApplyDepthMask	(bool& current, bool input)
{
	if (current != input)
	{
		current = input;
		glDepthMask(current);
	}
}

//---------------------------------------------------------------------
static void ApplyVertexBuffers(boost::shared_ptr<VertexBuffer> current[], const boost::shared_ptr<VertexBuffer> input[])
{
	for (size_t i = 0; i < RenderState::MaxVertexBuffers; ++i)
	{
		if (current[i] != input[i])
		{
			if (input[i])
			{
				current[i] = input[i];
				current[i]->Enable();
			}
			else
			{
				current[i]->Disable();
				current[i] = input[i];
			}
		}
	}
}
