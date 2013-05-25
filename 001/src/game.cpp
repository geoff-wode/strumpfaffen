#include <SDL.h>
#include <game.h>
#include <gl/gl_loader.h>
#include <buffers.h>
#include <shaders.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <display.h>

//----------------------------------------------------------

static void HandleInput(bool* keepRunning);

//----------------------------------------------------------

const glm::vec3 positions[] =
{
    glm::vec3(-1.0f,-1.0f,-1.0f),
    glm::vec3(-1.0f,-1.0f, 1.0f),
    glm::vec3(-1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f, 1.0f,-1.0f),
    glm::vec3(-1.0f,-1.0f,-1.0f),
    glm::vec3(-1.0f, 1.0f,-1.0f),
    glm::vec3(1.0f,-1.0f, 1.0f),
    glm::vec3(-1.0f,-1.0f,-1.0f),
    glm::vec3(1.0f,-1.0f,-1.0f),
    glm::vec3(1.0f, 1.0f,-1.0f),
    glm::vec3(1.0f,-1.0f,-1.0f),
    glm::vec3(-1.0f,-1.0f,-1.0f),
    glm::vec3(-1.0f,-1.0f,-1.0f),
    glm::vec3(-1.0f, 1.0f, 1.0f),
    glm::vec3(-1.0f, 1.0f,-1.0f),
    glm::vec3(1.0f,-1.0f, 1.0f),
    glm::vec3(-1.0f,-1.0f, 1.0f),
    glm::vec3(-1.0f,-1.0f,-1.0f),
    glm::vec3(-1.0f, 1.0f, 1.0f),
    glm::vec3(-1.0f,-1.0f, 1.0f),
    glm::vec3(1.0f,-1.0f, 1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f,-1.0f,-1.0f),
    glm::vec3(1.0f, 1.0f,-1.0f),
    glm::vec3(1.0f,-1.0f,-1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f,-1.0f, 1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f, 1.0f,-1.0f),
    glm::vec3(-1.0f, 1.0f,-1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(-1.0f, 1.0f,-1.0f),
    glm::vec3(-1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(-1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f,-1.0f, 1.0f)
};
const unsigned int triangleCount = sizeof(positions) / sizeof(positions[0]);

//----------------------------------------------------------

Game::Game()
	: running(true),
		model(1.0f)
{
}

Game::~Game()
{
	glDeleteBuffers(1, &positionsBuffer);
	glDeleteVertexArrays(1, &vao);
}

bool Game::Init()
{
	display.cameraPos = glm::vec3(4,3,3);
	glClearColor(0, 0, 0, 1);
	return true;
}

bool Game::Load()
{
	shader = Shader::New();
	if (shader)
	{
		if (!shader->Load("simple.vs.glsl", "simple.fs.glsl"))
		{
			return false;
		}

		//shader->SetParam("outColour", glm::vec3(1));
		mvpParam = shader->GetParamIndex("ModelViewProjection");

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		NewVertexBuffer(sizeof(positions), positions, &positionsBuffer);
	}

	return true;
}

void Game::Update(float elapsedMS)
{
	HandleInput(&running);
	if (IsRunning())
	{
		static float angle = 0.0f;
		model = glm::rotate(angle, glm::vec3(0,1,0));
		angle += (100 * elapsedMS);
		shader->SetParam(mvpParam, display.viewProjection * model);
	}
}

void Game::Render(float elapsedMS)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader->Apply();

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, positionsBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, triangleCount * 3);

	glDisableVertexAttribArray(0);
	glUseProgram(0);
}

static void HandleInput(bool* keepRunning)
{
	SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
    case SDL_QUIT: *keepRunning = false; break;
		case SDL_KEYDOWN: if (event.key.keysym.sym == SDLK_ESCAPE) { *keepRunning = false; }; break;
    default: break;
    }
  }
}
