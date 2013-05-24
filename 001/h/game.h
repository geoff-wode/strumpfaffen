#if ! defined(__GAME_H__)
#define __GAME_H__

#include <glm/glm.hpp>
#include <gl/gl_loader.h>
#include <shaders.h>

class Game
{
public:
	Game();
	~Game();

	bool Init();
	bool Load();
	void Update(float elapsedMS);
	void Render(float elapsedMS);

	bool IsRunning() const { return running; }

private:
	bool running;
	GLuint vao;
	GLuint positionsBuffer;
	boost::shared_ptr<Shader> shader;
	glm::mat4 model;
	size_t mvpParam;
};

#endif __GAME_H__
