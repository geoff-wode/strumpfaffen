#include <SDL.h>
#include <program.h>
#include <keyboard.h>
#include <boost/make_shared.hpp>
#include <textures/texture2d.h>

//----------------------------------------------------------------------------------

struct VertexPosition
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textureCoord;

	static const VertexDeclaration* GetVertexDecl()
	{
		static const VertexAttribute attrs[] =
		{
			{ "VertexPosition", GL_FLOAT_VEC3, offsetof(VertexPosition, position) },
			{ "VertexNormal", GL_FLOAT_VEC3, offsetof(VertexPosition, normal) },
			{ "VertexTexCoord0", GL_FLOAT_VEC2, offsetof(VertexPosition, textureCoord) }
		};
		static const size_t attrCount = sizeof(attrs) / sizeof(attrs[0]);
		static VertexDeclaration vertexDecl(sizeof(VertexPosition), attrs, attrCount);

		return &vertexDecl;
	}
};

//----------------------------------------------------------------------------------

static void DefineVertices(int width, int height, std::vector<VertexPosition>& vertices)
{
	vertices.resize(width * height);

	int i = 0;
	for (int z = 0; z < height; z++)
	{
		for (int x = 0; x < width; ++x)
		{
			vertices[i].position = glm::vec3(x, 0, -z);
			vertices[i].normal = glm::vec3(0, 0, 1);
			vertices[i].textureCoord = glm::vec2((float)x / (float)width, (float)z / (float)height);
			++i;
		}
	}
}

static void DefineIndices(int width, int height, std::vector<unsigned short>& indices)
{
	indices.resize(2 * width * (height - 1));

	int i = 0;
	int z = 0;

	while (z < height - 1)
	{
		for (int x = 0; x < width; ++x)
		{
			indices[i++] = x + (z * width);
			indices[i++] = x + ((z + 1) * width);
		}
		++z;

		if (z < height - 1)
		{
			for (int x = width - 1; x >= 0; --x)
			{
				indices[i++] = x + ((z + 1) * width);
				indices[i++] = x + (z * width);
			}
		}
		++z;
	}
}

//----------------------------------------------------------------------------------

Program::Program(boost::shared_ptr<Device> device)
	: quit(false), device(device)
{
}

Program::~Program()
{
}

//----------------------------------------------------------------------------------
extern void test();

void Program::Run()
{
	if (!Initialise()) { return; }

	test();

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

		device->SwapBuffers();
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

	turningShader.angle = 0.0f;

	return true;
}

//----------------------------------------------------------------------------------
void Program::LoadResources()
{
	const int width = 32;
	const int height = 32;

	vertexArray = boost::make_shared<VertexArray>();

	std::vector<unsigned short> indices;
	DefineIndices(width, height, indices);
	const size_t indexBufferSize = indices.size() * sizeof(indices[0]);
	
	std::vector<VertexPosition> vertices;
	DefineVertices(width, height, vertices);
	const size_t vertexBufferSize = vertices.size() * sizeof(vertices[0]);

	vertexArray->indexBuffer = boost::make_shared<Buffer>(indexBufferSize, GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);
	vertexArray->indexBuffer->Bind();
	vertexArray->indexBuffer->SetData(indices.data(), indexBufferSize, 0);
	vertexArray->indexBuffer->Unbind();

	vertexArray->vertexBuffer = boost::make_shared<Buffer>(vertexBufferSize, GL_ARRAY_BUFFER, GL_STATIC_DRAW);
	vertexArray->vertexBuffer->Bind();
	vertexArray->vertexBuffer->SetData(vertices.data(), vertexBufferSize, 0);
	vertexArray->vertexBuffer->Unbind();
	
	vertexArray->vertexDecl = VertexPosition::GetVertexDecl();

	renderState.vertexArray = vertexArray;

	turningShader.shader = boost::make_shared<ShaderProgram>("shaders/turning");
	if (turningShader.shader->Load())
	{
		turningShader.angleParam = turningShader.shader->GetUniforms().at("angle");
	}
	else
	{
		quit = true;
	}
}

//----------------------------------------------------------------------------------
void Program::Update(unsigned int elapsedMS)
{
	if (Keyboard::IsKeyDown(SDL_SCANCODE_ESCAPE)) { Quit(); }

	const float turnRate = 3.14159f / 1000.0f;
	turningShader.angle += turnRate * (float)elapsedMS;
	turningShader.angleParam->Set(turningShader.angle);
}

//----------------------------------------------------------------------------------
void Program::Render()
{
	device->Clear(clearState);

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
