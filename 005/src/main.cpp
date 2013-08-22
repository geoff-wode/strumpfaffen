#include <SDL.h>
#include <cstdlib>
#include <ctime>
#include <gl_loader/gl_loader.h>
#include <debug.h>
#include <device.h>
#include <buffers.h>
#include <keyboard.h>
#include <vertexarray.h>

//----------------------------------------------------------------------------------

struct Vertex
{
	glm::vec3 position;
	glm::vec2 textureCoord;
};

//----------------------------------------------------------------------------------

static bool Initialise();
static void HandleEvents();
static void UpdateGame(unsigned int elapsedMS);
static void RenderFrame();

//----------------------------------------------------------------------------------

FILE* debugLogFile;
static bool quit = false;
static KeyboardState oldKeyState;
static ClearState clearState;
static RenderState triangleRenderState;

//----------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	atexit(SDL_Quit);

	debugLogFile = fopen("stderr.txt", "wb");
	{
		char buffer[128];
		const time_t raw(time(NULL));
		const struct tm* local(localtime(&raw));
		strftime(buffer, sizeof(buffer)-1, "%c\n", local);
		LOG("%s", buffer);
	}

	if (!Initialise()) { exit(EXIT_FAILURE); }

	unsigned int lastUpdate = 0;
	while (!quit)
	{
		HandleEvents();

		unsigned int now = SDL_GetTicks();
		unsigned int elapsedMS = now - lastUpdate;
		lastUpdate = now;
		UpdateGame(elapsedMS);

		RenderFrame();

		Device::SwapBuffers();
	}

	return 0;
}

//----------------------------------------------------------------------------------
static bool Initialise()
{
	if (!Device::Initialise()) { return false; }

	static const VertexAttribute attrs[] =
	{
		VertexAttribute("Position", GL_FLOAT, 3, offsetof(Vertex, position)),
		VertexAttribute("TexCoord0", GL_FLOAT, 2, offsetof(Vertex, textureCoord))
	};

	static const Vertex vertices[] =
	{
		{ glm::vec3(-0.75f, -0.75f, 0.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3( 0.75f, -0.75f, 0.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3( 0.00f,  0.75f, 0.0f), glm::vec2(0.5f, 1.0f) }
	};

	// Create a vertex array of 1 buffer of 3 vertices of 2 attributes...
	boost::shared_ptr<VertexBuffer> vb = CreateVertexBuffer(sizeof(vertices), GL_STATIC_DRAW);
	vb->Enable();
	vb->SetData(vertices, sizeof(vertices), 0);
	VertexBuffer::Disable();

	VertexDeclarationPtr decl(new VertexDeclaration(sizeof(Vertex), vb, attrs, 2));
	triangleRenderState.vertexArray = boost::make_shared<VertexArray>(&decl, 1);

	triangleRenderState.shader = boost::make_shared<Shader>("shaders/passthru");
	if (!triangleRenderState.shader->Build()) { return false; }

	oldKeyState = Keyboard::GetState();
	clearState.colour = glm::vec4(0,0,1,1);

	return true;
}

//----------------------------------------------------------------------------------
static void UpdateGame(unsigned int elapsedMS)
{
	KeyboardState keyState = Keyboard::GetState();

	if (keyState->IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		quit = true;
		return;
	}
	
	oldKeyState = keyState;
}

//----------------------------------------------------------------------------------
static void RenderFrame()
{
	Device::Clear(clearState, GL_COLOR_BUFFER_BIT);
	Device::Draw(GL_TRIANGLES, 1, 0, triangleRenderState);
}

//----------------------------------------------------------------------------------
static void HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT: quit = true; break;
		default: break;
		}
	}
}

//----------------------------------------------------------------------------------
