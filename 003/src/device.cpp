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
	ApplyRenderState(state);
	switch (primitiveType)
	{
	case GL_TRIANGLES: glDrawArrays(primitiveType, 0, primitiveCount * 3); break;
	case GL_TRIANGLE_STRIP: glDrawArrays(primitiveType, 0, primitiveCount + 2); break;
	default: break;
	}
}

void Device::DrawIndexed(GLenum primitiveType, size_t primitiveCount, GLenum indexType, const RenderState& state)
{
	ApplyRenderState(state);

	switch (primitiveType)
	{
	case GL_TRIANGLES: glDrawElements(primitiveType, primitiveCount * 3, indexType, 0); break;
	case GL_TRIANGLE_STRIP: glDrawElements(primitiveType, primitiveCount + 2, indexType, 0); break;
	default: break;
	}
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

	ApplyColourMask(newState.colourMask);
	ApplyDepthMask(newState.depthMask);
}

//---------------------------------------------------------------------------------------

void Device::ApplyRenderState(const RenderState& state)
{
	ApplyColourMask(state.colourMask);
	ApplyDepthMask(state.depthMask);
	ApplyShaderProgram(state.shader);
	ApplyVertexBuffer(state.vertexArray->vertexBuffer);
	ApplyIndexBuffer(state.vertexArray->indexBuffer);
	ApplyVertexArray(state.vertexArray->vertexDecl);
}

void Device::ApplyColourMask(const glm::bvec4& mask)
{
	if (mask != mask)
	{
		renderState.colourMask = mask;
		glColorMask(mask.r, mask.g, mask.b, mask.a);
	}
}

void Device::ApplyDepthMask(bool mask)
{
	if (renderState.depthMask != mask)
	{
		renderState.depthMask = mask;
		glDepthMask(mask);
	}
}

void Device::ApplyShaderProgram(boost::shared_ptr<ShaderProgram> shader)
{
	if (activeShader != shader)
	{
		activeShader = shader;
		activeShader->Bind();
	}
	activeShader->Apply();
}

void Device::ApplyVertexBuffer(boost::shared_ptr<Buffer> buffer)
{
	if (activeVertexBuffer != buffer)
	{
		if (activeVertexBuffer) { activeVertexBuffer->Unbind(); }
		activeVertexBuffer = buffer;
		activeVertexBuffer->Bind();
	}
}

void Device::ApplyIndexBuffer(boost::shared_ptr<Buffer> buffer)
{
	// If the buffers are different...
	if (activeIndexBuffer != buffer)
	{
		if (buffer) // ...then if a new buffer is being applied, bind to it...
		{
			activeIndexBuffer = buffer;
			activeIndexBuffer->Bind();
		}
		else // ...there is no buffer, unbind the current one...
		{
			activeIndexBuffer->Unbind();
			activeIndexBuffer = buffer;
		}
	}
}


// Herein we match up the name of the vertex declaration attribute with the shader attribute
// in order to determine its shader "location". Note that these locations may be different from
// shader to shader.
void Device::ApplyVertexArray(const VertexDeclaration* const decl)
{
	// Enable each attibute of the vertex declaration...
	const size_t stride = decl->Stride;
	const VertexAttributeList& attrs = decl->Attributes;

	for (VertexAttributeList::const_iterator i = decl->Attributes.cbegin(); i != decl->Attributes.cend(); ++i)
	{
		const ShaderAttributeMap::const_iterator attr = activeShader->GetAttributes()->find(i->name);
		if (attr != activeShader->GetAttributes()->cend())
		{
			// ...taking care to use the attribute location defined in the shader...
			const GLuint location = attr->second.location;

			glEnableVertexAttribArray(location);
			switch (i->type)
			{
			case GL_FLOAT: glVertexAttribPointer(location, 1, GL_FLOAT, GL_FALSE, stride, (const void*)i->offset); break;
			case GL_FLOAT_VEC2: glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, stride, (const void*)i->offset); break;
			case GL_FLOAT_VEC3: glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, stride, (const void*)i->offset); break;
			case GL_FLOAT_VEC4: glVertexAttribPointer(location, 4, GL_FLOAT, GL_FALSE, stride, (const void*)i->offset); break;
			default: break;
			}
		}
	}
}
