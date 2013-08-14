#include <SDL.h>
#include <device.h>
#include <gl_loader/gl_loader.h>

//---------------------------------------------------------------------------------------

static void InitialiseSDL();
static void ForceState(const ClearState& clearState, const RenderState& renderState);

//---------------------------------------------------------------------------------------

Device::Device(const glm::ivec2& backbufferSize)
	: backbufferSize(backbufferSize)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	atexit(SDL_Quit);
}

Device::~Device()
{
}

//---------------------------------------------------------------------------------------
bool Device::Initialise()
{
	InitialiseSDL();
	if (NULL == SDL_SetVideoMode(backbufferSize.x, backbufferSize.y, 32, SDL_OPENGL | SDL_HWSURFACE))
	{
		return false;
	}
	InitialiseSDL();

	viewport.size.x = SDL_GetVideoSurface()->w;
	viewport.size.y = SDL_GetVideoSurface()->h;

	ogl_LoadFunctions();

	ForceState(clearState, renderState);

	return true;
}

//---------------------------------------------------------------------------------------
void Device::SetViewport(const Viewport& viewport)
{
	if (this->viewport != viewport)
	{
		this->viewport = viewport;
		glViewport(viewport.topLeft.x, viewport.topLeft.y, viewport.size.x, viewport.size.y);
	}
}

Viewport Device::GetViewport() const
{
	return viewport;
}

//---------------------------------------------------------------------------------------

void Device::Clear(const ClearState& state)
{
	ApplyClearState(state);

	glClear(state.buffers);
}

//---------------------------------------------------------------------------------------

void Device::Draw(GLenum primitiveType, size_t primitiveCount, const RenderState& state)
{
	state.shader->Use();

	state.vertexBuffer->Bind();
	ApplyVertexDecl(state.vertexDecl);
	
	glDrawArrays(primitiveType, 0, primitiveCount * 3);

	state.vertexBuffer->Unbind();

	glUseProgram(0);
}

//---------------------------------------------------------------------------------------
static void InitialiseSDL()
{
	const struct SDLAttribute
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

//---------------------------------------------------------------------------------------
static void ForceState(const ClearState& clearState, const RenderState& renderState)
{
	glClearColor(clearState.colour.r, clearState.colour.g, clearState.colour.b, clearState.colour.a);
	glClearDepth(clearState.depth);

	glColorMask(renderState.colourMask.r, renderState.colourMask.g, renderState.colourMask.b, renderState.colourMask.a);
	glDepthMask(renderState.depthMask);
}

//---------------------------------------------------------------------------------------
void Device::ApplyClearState(const ClearState& newState)
{
	if (clearState.colour != newState.colour)
	{
		clearState.colour = newState.colour;
		glClearColor(clearState.colour.r, clearState.colour.g, clearState.colour.b, clearState.colour.a);
	}

	if (clearState.depth != newState.depth)
	{
		clearState.depth = newState.depth;
		glClearDepth(clearState.depth);
	}

	if (renderState.colourMask != newState.colourMask)
	{
		renderState.colourMask = newState.colourMask;
		glColorMask(renderState.colourMask.r, renderState.colourMask.g, renderState.colourMask.b, renderState.colourMask.a);
	}

	if (renderState.depthMask != newState.depthMask)
	{
		renderState.depthMask = newState.depthMask;
		glDepthMask(renderState.depthMask);
	}
}

//---------------------------------------------------------------------------------------

void Device::ApplyVertexDecl(const VertexDeclaration* const decl)
{
	if (renderState.vertexDecl != decl)
	{
		const size_t stride = decl->Stride;
		const VertexAttributeList& attrs = decl->Attributes;

		GLuint index = 0;
		for (VertexAttributeList::const_iterator i = decl->Attributes.cbegin(); i != decl->Attributes.cend(); ++i)
		{
			glEnableVertexAttribArray(index);
			switch (i->type)
			{
			case GL_FLOAT: glVertexAttribPointer(index, 1, GL_FLOAT, GL_FALSE, stride, (const void*)i->offset); break;
			case GL_FLOAT_VEC2: glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, stride, (const void*)i->offset); break;
			case GL_FLOAT_VEC3: glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, stride, (const void*)i->offset); break;
			case GL_FLOAT_VEC4: glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, stride, (const void*)i->offset); break;
			default: break;
			}
		}

		renderState.vertexDecl = decl;
	}
}
