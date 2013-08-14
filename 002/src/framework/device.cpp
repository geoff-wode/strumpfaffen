#include <SDL.h>
#include <gl_loader/gl_loader.h>
#include <framework/debug.h>
#include <framework/device.h>
#include <framework/shaders/shader.h>
#include <framework/buffers/buffers.h>

using namespace Framework;

//-------------------------------------------------------------------

static void InitialseSDL();

//-------------------------------------------------------------------

ShaderPtr Device::CreateShader(const std::string& name)
{
	ShaderPtr shader(new Shader(name, this));
	return shader;
}

//-------------------------------------------------------------------

Device::Device(const glm::ivec2& backbufferSize, bool fullScreen)
	: fullScreen(fullScreen),
		backbufferSize(backbufferSize)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	atexit(SDL_Quit);
}

Device::~Device()
{
}

bool Device::Initialise()
{
	InitialseSDL();
	if (NULL == SDL_SetVideoMode(backbufferSize.x, backbufferSize.y, 32,
		SDL_OPENGL | SDL_HWSURFACE | (fullScreen ? SDL_FULLSCREEN : 0)))
	{
		ASSERTM(false, "failed to create system window");
		return false;
	}

	// Some platforms have a bug wherein SDL state is trashed after calling SDL_SetVideoMode...
	InitialseSDL();

	// Initialse OpenGL library
	ogl_LoadFunctions();
  LOG("GL v%d.%d\n", ogl_GetMajorVersion(), ogl_GetMinorVersion());
  LOG("GLSL version: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	// Set default state(s)...
	clearState.ForceDefaults();
	drawState.renderState.ForceDefaults();
	
	return true;
}

void Device::Clear(const ClearState& clearState)
{
	this->clearState.Set(clearState);

	GLenum buffers = 0;
	if (clearState.buffers & ClearState::Buffers::Colour)
	{
		buffers |= GL_COLOR_BUFFER_BIT;
	}
	if (clearState.buffers & ClearState::Buffers::DepthStencil)
	{
		buffers |= (GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}
	glClear(buffers);
}

void Device::Render(const Framework::DrawState& drawState, size_t primitiveCount, PrimitiveType::Enum primitiveType)
{
	this->drawState.Set(drawState);

	size_t elementCount;
	switch (primitiveType)
	{
	case PrimitiveType::Triangles: elementCount = primitiveCount * 3; break;
	case PrimitiveType::TriangleStrip: elementCount = primitiveCount + 2; break;
	default: return;
	}

	if (this->drawState.indexBuffer)
	{
		glDrawElements(primitiveType, elementCount, this->drawState.indexBuffer->DataType, (const void*)0);
	}
	else
	{
		glDrawArrays(primitiveType, 0, elementCount);
	}
}

//-------------------------------------------------------------------

static void InitialseSDL()
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
