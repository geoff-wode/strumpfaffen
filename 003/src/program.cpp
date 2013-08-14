#include <SDL.h>
#include <program.h>
#include <keyboard.h>
#include <boost/make_shared.hpp>

//----------------------------------------------------------------------------------

struct VertexPosition
{
	glm::vec3 position;

	static const VertexDeclaration* GetVertexDecl()
	{
		static const VertexAttribute attrs[] =
		{
			{ "Position", GL_FLOAT_VEC3, offsetof(VertexPosition, position) }
		};
		static VertexDeclaration vertexDecl(sizeof(VertexPosition), attrs, 1);

		return &vertexDecl;
	}
};

//----------------------------------------------------------------------------------

Program::Program(boost::shared_ptr<Device> device)
	: quit(false), device(device)
{
}

Program::~Program()
{
}

//----------------------------------------------------------------------------------

void Program::Run()
{
	if (!Initialise()) { return; }
	LoadResources();

	unsigned int lastTime = 0;
	while (!quit)
	{
		HandleEvents();
		const unsigned int now = SDL_GetTicks();
		const unsigned int elapsedMS = now - lastTime;
		lastTime = now;
		Update(elapsedMS);
		Render();
		SDL_GL_SwapBuffers();
	}
}

//----------------------------------------------------------------------------------
void Program::Quit()
{
	quit = true;
	Quitting();
}

//----------------------------------------------------------------------------------
bool Program::Initialise()
{
	if (!device->Initialise()) { return false; }

	return true;
}

//----------------------------------------------------------------------------------
void Program::LoadResources()
{
	static const VertexPosition vertices[] =
	{
		{ glm::vec3(-0.75f,-0.75f, 0.0f) },
		{ glm::vec3( 0.75f,-0.75f, 0.0f) },
		{ glm::vec3( 0.0f, 0.75f, 0.0f) }
	};

	vertexBuffer = boost::make_shared<Buffer>(sizeof(vertices), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
	vertexBuffer->Bind();
	vertexBuffer->SetData(vertices, sizeof(vertices), 0);
	vertexBuffer->Unbind();

	renderState.vertexDecl = VertexPosition::GetVertexDecl();
	renderState.vertexBuffer = vertexBuffer;

	shader = boost::make_shared<ShaderProgram>("shaders/basic");
	renderState.shader = shader.get();
}

//----------------------------------------------------------------------------------
void Program::Update(unsigned int elapsedMS)
{
	if (Keyboard::IsKeyDown(SDLK_ESCAPE)) { Quit(); }
}

//----------------------------------------------------------------------------------
void Program::Render()
{
	device->Clear(clearState);
	device->Draw(GL_TRIANGLES, 1, renderState);
}

//----------------------------------------------------------------------------------
void Program::Quitting()
{
}

//----------------------------------------------------------------------------------
void Program::HandleEvents()
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
