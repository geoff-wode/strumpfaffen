#include <SDL.h>
#include <exception>
#include <gl_loader/gl_loader.h>

#include <core/debug.h>
#include <core/device.h>

using namespace Core;

//------------------------------------------------------------------

struct Device::DeviceImpl
{
	DeviceImpl()
		: fullScreen(false),
			screenWidth(800), screenHeight(600)
	{
	}

	bool fullScreen;
	unsigned int screenWidth;
	unsigned int screenHeight;
	ClearState clearState;
	const ClearState defaultClearState;
	Framework::Viewport viewport;
};

//------------------------------------------------------------------

static void InitSDL();
static void ForceClearState(const ClearState& state);

//------------------------------------------------------------------

unsigned int Device::MaxVertexAttributes()
{
	static GLint maxVertexAttributes = 0;

	if (0 == maxVertexAttributes)
	{
		glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttributes);
	}

	return maxVertexAttributes;
}

Device::Device()
	: impl(new DeviceImpl())
{
}

Device::~Device()
{
}

void Device::BackbufferWidth(unsigned int value)
{
	impl->screenWidth = value;
}

void Device::BackbufferHeight(unsigned int value)
{
	impl->screenHeight = value;
}

unsigned int Device::BackbufferWidth() const
{
	return impl->screenWidth;
}

unsigned int Device::BackbufferHeight() const
{
	return impl->screenHeight;
}

bool Device::Initialise()
{
	InitSDL();
	if (NULL == SDL_SetVideoMode(impl->screenWidth, impl->screenHeight, 32,
		SDL_OPENGL | SDL_HWSURFACE | (impl->fullScreen ? SDL_FULLSCREEN : 0)))
	{
		LOG("failed to create system window");
		return false;
	}
	// Some platforms have a bug wherein SDL state is trashed after calling SDL_SetVideoMode...
	InitSDL();

	ogl_LoadFunctions();
  LOG("GL v%d.%d\n", ogl_GetMajorVersion(), ogl_GetMinorVersion());
  LOG("GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	// Make sure the GL internal state is correct:
	ForceClearState(impl->clearState);

	SetViewport(impl->viewport);

	return true;
}

void Device::Clear()
{
	Clear(impl->defaultClearState);
}

void Device::Clear(const ClearState& state)
{
	if (state.Colour != impl->clearState.Colour)
	{
		impl->clearState.Colour = state.Colour;
		glClearColor(state.Colour.r, state.Colour.g, state.Colour.b, state.Colour.a);
	}

	if (state.ColourMask != impl->clearState.ColourMask)
	{
		impl->clearState.ColourMask = state.ColourMask;
		glColorMask(state.ColourMask.r, state.ColourMask.g, state.ColourMask.b, state.ColourMask.a);
	}

	if (state.Depth != impl->clearState.Depth)
	{
		impl->clearState.Depth = state.Depth;
		glClearDepth(state.Depth);
	}

	if (state.DepthMask != impl->clearState.DepthMask)
	{
		impl->clearState.DepthMask = state.DepthMask;
		glDepthMask(state.DepthMask);
	}

	switch (state.Buffers)
	{
	case ClearBuffers::Colour: glClear(GL_COLOR_BUFFER_BIT); break;
	case ClearBuffers::DepthStencil: glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); break;
	case ClearBuffers::All: glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); break;
	}
}

Framework::Viewport	Device::GetViewport() const
{
	return impl->viewport;
}

void Device::SetViewport(const Framework::Viewport& value)
{
	glViewport(
		impl->viewport.LowerLeft.x, impl->viewport.LowerLeft.y,
		impl->viewport.Dimensions.x, impl->viewport.Dimensions.y);
	impl->viewport = value;
}


//------------------------------------------------------------------

static void InitSDL()
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


static void ForceClearState(const ClearState& state)
{
	glClearColor(state.Colour.r, state.Colour.g, state.Colour.b, state.Colour.a);
	glColorMask(state.ColourMask.r, state.ColourMask.g, state.ColourMask.b, state.ColourMask.a);

	glClearDepth(state.Depth);
	glDepthMask(state.DepthMask);
}
