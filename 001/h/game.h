#if ! defined(__GAME_H__)
#define __GAME_H__

#include <glm/glm.hpp>
#include <gl/gl_loader.h>
#include <shaders.h>
#include <model.h>

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
	boost::shared_ptr<Shader> shader;
	boost::shared_ptr<Model> model;
	glm::mat4 modelMatrix;
	size_t mvpParam;
};

#endif __GAME_H__
