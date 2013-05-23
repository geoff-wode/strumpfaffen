#include <SDL.h>
#include <game.h>
#include <gl/gl_loader.h>
#include <glm/glm.hpp>
#include <buffers.h>
#include <shaders.h>

//----------------------------------------------------------

static void HandleInput(bool* keepRunning);

//----------------------------------------------------------

const float positions[] =
{
	0.0f, 0.75f, 0.0f,
	-0.75f, -0.75f, 0.0f,
	0.75f, -0.75f, 0.0f
};

//----------------------------------------------------------

Game::Game()
	: running(true)
{
}

Game::~Game()
{
	glDeleteBuffers(1, &positionsBuffer);
	glDeleteProgram(shader);
	glDeleteVertexArrays(1, &vao);
}

bool Game::Init()
{
	glClearColor(0, 0, 1, 1);
	return true;
}

bool Game::Load()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	NewVertexBuffer(sizeof(positions), positions, &positionsBuffer);

	if (!NewShaderProgram("passthru.vs.glsl", "red.fs.glsl", &shader))
	{
		running = false;
	}

	return running;
}

void Game::Update(float elapsedMS)
{
	HandleInput(&running);
	if (IsRunning())
	{
	}
}

void Game::Render(float elapsedMS)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shader);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, positionsBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);

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
