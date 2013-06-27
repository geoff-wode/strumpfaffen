#include <SDL.h>
#include <game.h>
#include <gl/gl_loader.h>
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
	: running(true)
{
}

Game::~Game()
{
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
	shader->Load("simple.vs.glsl", "simple.fs.glsl");
	mvpParam = shader->GetParameter("ModelViewProjection");
	textureSampler = shader->GetParameter("textureSampler");

	model = Model::New();
	model->Load("monkey.obj");

	texture = Texture::New(0);
	texture->Load("monkey.DDS");

	return true;
}

void Game::Update(float elapsedMS)
{
	HandleInput(&running);
	if (IsRunning())
	{
		static float angle = 0.0f;
		modelMatrix = glm::rotate(angle, glm::vec3(0,1,0));
		angle += (100 * elapsedMS);
		shader->SetParam(mvpParam, display.viewProjection * modelMatrix);
	}
}

void Game::Render(float elapsedMS)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader->Apply();
//	texture->Apply();
	model->Render();
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
