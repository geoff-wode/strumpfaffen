#include <vector>
#include <SDL.h>
#include <gl_loader/gl_loader.h>
#include <device.h>
#include <debug.h>
#include <buffers.h>

//---------------------------------------------------

// Define the layout of the uniform buffer...
// NOTE: Keep this in lock-step with the block definition in shader.cpp::CommonShaderSrc.
struct UniformBufferData
{
	glm::vec4 cameraPos;				// Set under user control.
	glm::mat4 worldMatrix;			// Set under user control.
	glm::mat4 viewMatrix;				// Set under user control.
	glm::mat4 projectionMatrix;	// Set under user control.

	// Set automatically...
	glm::mat4 inverseWorldMatrix;
	glm::mat4 inverseViewMatrix;
	glm::mat4 worldViewMatrix;
	glm::mat4 worldViewProjectionMatrix;
	glm::mat4 viewProjectionMatrix;
};

struct GLState
{
	ClearState clearState;
	RenderState renderState;
};

//---------------------------------------------------

static void InitialiseGLState();
static void OnExit();
static void ConfigureGL();
static void ApplyColourMask(const glm::bvec4& value);
static void ApplyDepthMask(bool value);
static void ApplyIndexBuffer(boost::shared_ptr<IndexBuffer> buffer);
static void ApplyUniformBuffer();
static void ApplyShader(boost::shared_ptr<Shader> value);
static void ApplyVertexArray(boost::shared_ptr<VertexArray> value);

//---------------------------------------------------

static SDL_Window* mainWindow;
static SDL_GLContext glContext;
static GLState glState;
static ShaderUniformMap uniformBlock;
static bool uniformBufferChanged = false;
static UniformBufferData uniformBufferData;
static boost::shared_ptr<UniformBuffer> uniformBuffer;

//---------------------------------------------------

bool Device::Initialise()
{
	atexit(OnExit);

	ConfigureGL();

	mainWindow = SDL_CreateWindow("Strumpfaffen", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	if (!mainWindow)
	{
		LOG("SDL_CreateWindow: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	glContext = SDL_GL_CreateContext(mainWindow);
	if (!glContext)
	{
		LOG("SDL_CreateWindow: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	ogl_LoadFunctions();
  LOG("OpenGL v%d.%d\n", ogl_GetMajorVersion(), ogl_GetMinorVersion());
  LOG("GLSL v%s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	InitialiseGLState();

	return true;
}

//---------------------------------------------------

void Device::SetWorldMatrix(const glm::mat4 value)
{
	if (uniformBufferData.worldMatrix != value)
	{
		uniformBufferData.worldMatrix = value;
		uniformBufferChanged = true;
	}
}

void Device::SetViewMatrix(const glm::mat4& value)
{
	if (uniformBufferData.viewMatrix != value)
	{
		uniformBufferData.viewMatrix = value;
		uniformBufferChanged = true;
	}
}

void Device::SetProjectionMatrix(const glm::mat4& value)
{
	if (uniformBufferData.projectionMatrix != value)
	{
		uniformBufferData.projectionMatrix = value;
		uniformBufferChanged = true;
	}
}

//---------------------------------------------------

void Device::Clear(const ClearState& clearState, GLenum buffers)
{
	ApplyColourMask(clearState.colourMask);
	ApplyDepthMask(clearState.depthMask);

	if (glState.clearState.colour != clearState.colour)
	{
		glState.clearState.colour = clearState.colour;
		glClearColor(
			glState.clearState.colour.r,
			glState.clearState.colour.g,
			glState.clearState.colour.b,
			glState.clearState.colour.a);
	}

	if (glState.clearState.depth != clearState.depth)
	{
		glState.clearState.depth = clearState.depth;
		glClearDepth(glState.clearState.depth);
	}

	glClear(buffers);
}

//---------------------------------------------------

void Device::Draw(GLenum primitiveType, size_t primitiveCount, size_t startVertex, const RenderState& renderState)
{
	ApplyUniformBuffer();
	ApplyColourMask(renderState.colourMask);
	ApplyDepthMask(renderState.depthMask);
	ApplyVertexArray(renderState.vertexArray);
	ApplyIndexBuffer(renderState.indexBuffer);
	ApplyShader(renderState.shader);

	size_t vertexCount = 0;
	switch (primitiveType)
	{
	case GL_TRIANGLES: vertexCount = primitiveCount * 3; break;
	case GL_TRIANGLE_STRIP: vertexCount = primitiveCount + 2; break;
	default: return;
	}

	if (glState.renderState.indexBuffer)
	{
		glDrawElements(primitiveType, vertexCount, glState.renderState.indexBuffer->Type, (const void*)startVertex);
	}
	else
	{
		glDrawArrays(primitiveType, startVertex, vertexCount);
	}
}

//---------------------------------------------------

void Device::SwapBuffers()
{
	SDL_GL_SwapWindow(mainWindow);
}

//---------------------------------------------------

static void InitialiseGLState()
{
	glClearColor(
		glState.clearState.colour.r,
		glState.clearState.colour.g,
		glState.clearState.colour.b,
		glState.clearState.colour.a);

	glClearDepth(glState.clearState.depth);

	glColorMask(
		glState.renderState.colourMask.r,
		glState.renderState.colourMask.g,
		glState.renderState.colourMask.b,
		glState.renderState.colourMask.a);		
	
	glDepthMask(glState.renderState.depthMask);
}

//---------------------------------------------------

static void OnExit()
{
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(mainWindow);
}

//---------------------------------------------------

static void ConfigureGL()
{
  static const struct SDLAttribute
  {
    SDL_GLattr  attr;
    int         value;
  } sdlAttributes[] =
  {
    { SDL_GL_DOUBLEBUFFER,       1 },
    { SDL_GL_RED_SIZE,           8 },
    { SDL_GL_GREEN_SIZE,         8 },
    { SDL_GL_BLUE_SIZE,          8 },
    { SDL_GL_ALPHA_SIZE,         8 },
    { SDL_GL_BUFFER_SIZE,        24 },
    { SDL_GL_MULTISAMPLEBUFFERS, 1 },
    { SDL_GL_MULTISAMPLESAMPLES, 4 }
  };
  const int NumAttributes = sizeof(sdlAttributes)/sizeof(sdlAttributes[0]);
  for (int i = 0; i < NumAttributes; ++i)
  {
    SDL_GL_SetAttribute(sdlAttributes[i].attr, sdlAttributes[i].value);
  }
}

//---------------------------------------------------
static void ApplyShader(boost::shared_ptr<Shader> value)
{
	if (glState.renderState.shader != value)
	{
		glState.renderState.shader = value;
		glState.renderState.shader->Enable();
	}
	glState.renderState.shader->ApplyUniforms();
}

//---------------------------------------------------
static void ApplyColourMask(const glm::bvec4& value)
{
	// The colour mask controls rendering as well as clearing, so the value
	// is maintained as part of the _render state_ NOT the clear state.
	if (glState.renderState.colourMask != value)
	{
		glColorMask(value.r, value.g, value.b, value.a);
		glState.renderState.colourMask = value;
	}
}

//---------------------------------------------------
static void ApplyDepthMask(bool value)
{
	// The depth mask controls rendering as well as clearing, so the value
	// is maintained as part of the _render state_ NOT the clear state.
	if (glState.renderState.depthMask != value)
	{
		glDepthMask(value);
		glState.renderState.depthMask = value;
	}
}

//---------------------------------------------------
static void ApplyIndexBuffer(boost::shared_ptr<IndexBuffer> buffer)
{
	if (glState.renderState.indexBuffer != buffer)
	{
		if (buffer)
		{
			buffer->Enable();
			glState.renderState.indexBuffer = buffer;
		}
		else
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glState.renderState.indexBuffer = NULL;
		}
	}
}

//---------------------------------------------------
static void ApplyUniformBuffer()
{
	if (uniformBufferChanged)
	{
		uniformBufferData.inverseWorldMatrix				= glm::inverse(uniformBufferData.worldMatrix);
		uniformBufferData.worldViewMatrix						= uniformBufferData.viewMatrix * uniformBufferData.worldMatrix;
		uniformBufferData.viewProjectionMatrix			= uniformBufferData.projectionMatrix * uniformBufferData.viewMatrix;
		uniformBufferData.worldViewProjectionMatrix = uniformBufferData.projectionMatrix * uniformBufferData.worldViewMatrix;

		uniformBuffer->Enable();
		uniformBuffer->SetData(&uniformBufferData, sizeof(uniformBufferData), 0);
		uniformBuffer->Disable();

		uniformBufferChanged = false;
	}
}

//---------------------------------------------------
static void ApplyVertexArray(boost::shared_ptr<VertexArray> value)
{
	if (glState.renderState.vertexArray != value)
	{
		glState.renderState.vertexArray = value;
		glState.renderState.vertexArray->Enable();
	}
}
