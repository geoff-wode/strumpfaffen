#include <SDL.h>
#include <program.h>
#include <keyboard.h>
#include <boost/make_shared.hpp>

//----------------------------------------------------------------------------------

struct VertexPosition
{
	glm::vec3 position;
	glm::vec3 colour;

	static const VertexDeclaration* GetVertexDecl()
	{
		static const VertexAttribute attrs[] =
		{
			{ "VertexPosition", GL_FLOAT_VEC3, offsetof(VertexPosition, position) },
			{ "VertexColour", GL_FLOAT_VEC3, offsetof(VertexPosition, colour) }
		};
		static const size_t attrCount = sizeof(attrs) / sizeof(attrs[0]);
		static VertexDeclaration vertexDecl(sizeof(VertexPosition), attrs, attrCount);

		return &vertexDecl;
	}
};

//----------------------------------------------------------------------------------

static const VertexPosition vertices[] =
{
	{ glm::vec3(-0.75f,-0.75f, 0.0f),	glm::vec3(1,0,0) },
	{ glm::vec3( 0.75f,-0.75f, 0.0f), glm::vec3(0,1,0) },
	{ glm::vec3( 0.0f, 0.75f, 0.0f),	glm::vec3(0,0,1) }
};
static const size_t vertexCount = sizeof(vertices) / sizeof(vertices[0]);

static const unsigned short indices[] = { 0, 1, 2 };
static const size_t indexCount = sizeof(indices) / sizeof(indices[0]);

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

	unsigned int lastUpdate = 0;

	while (!quit)
	{
		HandleEvents();

		unsigned int now = SDL_GetTicks();
		unsigned int elapsedMS = now - lastUpdate;
		lastUpdate = now;
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
	clearState.colour = glm::vec4(0);

	angle = 0.0f;

	return true;
}

//----------------------------------------------------------------------------------
void Program::LoadResources()
{
	vertexArray = boost::make_shared<VertexArray>();

	vertexArray->indexBuffer = boost::make_shared<Buffer>(sizeof(indices), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
	vertexArray->indexBuffer->Bind();
	vertexArray->indexBuffer->SetData(indices, sizeof(indices), 0);
	vertexArray->indexBuffer->Unbind();

	vertexArray->vertexBuffer = boost::make_shared<Buffer>(sizeof(vertices), GL_ARRAY_BUFFER, GL_STATIC_DRAW);
	vertexArray->vertexBuffer->Bind();
	vertexArray->vertexBuffer->SetData(vertices, sizeof(vertices), 0);
	vertexArray->vertexBuffer->Unbind();
	
	vertexArray->vertexDecl = VertexPosition::GetVertexDecl();

	renderState.vertexArray = vertexArray;

	colourShader.shader = boost::make_shared<ShaderProgram>("shaders/coloured");
	colourShader.shader->GetUniforms().at("colourToUse")->Set(glm::vec4(0.3f,0.2f,0.6f,1.0f));

	turningShader.shader = boost::make_shared<ShaderProgram>("shaders/turning");
	turningShader.angle = turningShader.shader->GetUniforms().at("angle");
}

//----------------------------------------------------------------------------------
void Program::Update(unsigned int elapsedMS)
{
	if (Keyboard::IsKeyDown(SDLK_ESCAPE)) { Quit(); }

	const float turnRate = 3.14159f / 1000.0f;
	angle += turnRate * (float)elapsedMS;
	turningShader.angle->Set(angle);
}

//----------------------------------------------------------------------------------
void Program::Render()
{
	device->Clear(clearState);

	renderState.shader = colourShader.shader;
	device->DrawIndexed(GL_TRIANGLES, 1, GL_UNSIGNED_SHORT, renderState);

	renderState.shader = turningShader.shader;
	device->DrawIndexed(GL_TRIANGLE_STRIP, 1, GL_UNSIGNED_SHORT, renderState);
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
