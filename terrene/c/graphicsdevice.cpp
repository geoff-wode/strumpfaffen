#include <SDL.h>
#include <debug.h>
#include <graphicsdevice.h>
#include <gl_loader/gl_loader.h>

using namespace Terrene;

static void InitSDL();
static void ForceClearState(const ClearState& state);
static void ApplyClearColour(ClearState& currentState, const ClearState& newState);
static void ApplyClearDepth(ClearState& currentState, const ClearState& newState);

GraphicsDevice::GraphicsDevice()
	: FullScreen(false), BackbufferWidth(800), BackbufferHeight(600)
{
}

bool GraphicsDevice::Initialise()
{
	InitSDL();
	if (NULL == SDL_SetVideoMode(BackbufferWidth, BackbufferHeight, 32, SDL_OPENGL | SDL_HWSURFACE | ((FullScreen ? SDL_FULLSCREEN : 0))))
  {
		LOG("failed to set video mode\n");
    return false;
  }
	InitSDL(); // workaround for occassional bug in some SDL implementations
	ogl_LoadFunctions();
  LOG("GL v%d.%d\n", ogl_GetMajorVersion(), ogl_GetMinorVersion());
  LOG("GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	const Viewport defaultViewport(0, 0, BackbufferWidth, BackbufferHeight);
	SetViewport(defaultViewport);

	ForceClearState(currentClearState);

	return true;
}

void GraphicsDevice::SetViewport(const Viewport& viewport)
{
	if (	(currentViewport.X != viewport.X)
		||	(currentViewport.Y != viewport.Y)
		||	(currentViewport.Width != viewport.Width)
		||	(currentViewport.Height != viewport.Height)
		)
	{
		currentViewport = viewport;
		glViewport(0, 0, (GLsizei)currentViewport.Width, (GLsizei)currentViewport.Height);
	}
}

Viewport GraphicsDevice::GetViewport() const
{
	return currentViewport;
}

void GraphicsDevice::Clear(const ClearState& state)
{
	ApplyClearColour(currentClearState, state);
	ApplyClearDepth(currentClearState, state);

	GLbitfield clearBuffers = 0;
	if (state.Buffers & ClearBuffers::Colour) { clearBuffers |= GL_COLOR_BUFFER_BIT; }
	if (state.Buffers & ClearBuffers::Depth) { clearBuffers |= GL_DEPTH_BUFFER_BIT; }
	if (state.Buffers & ClearBuffers::Stencil) { clearBuffers |= GL_STENCIL_BUFFER_BIT; }
	glClear(clearBuffers);
}

static void ApplyClearColour(ClearState& currentState, const ClearState& newState)
{
	if (currentState.ClearColour != newState.ClearColour)
	{
		currentState.ClearColour = newState.ClearColour;
		glClearColor(currentState.ClearColour.r, currentState.ClearColour.g, currentState.ClearColour.b, currentState.ClearColour.a);
	}
}

static void ApplyClearDepth(ClearState& currentState, const ClearState& newState)
{
	if (currentState.Depth != newState.Depth)
	{
		currentState.Depth = newState.Depth;
		glClearDepth(currentState.Depth);
	}
}

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
	glClearColor(state.ClearColour.r, state.ClearColour.g, state.ClearColour.b, state.ClearColour.a);
	glClearDepth(state.Depth);
}
