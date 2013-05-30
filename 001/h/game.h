#if ! defined(__GAME_H__)
#define __GAME_H__

#include <glm/glm.hpp>
#include <gl/gl_loader.h>
#include <shaders.h>
#include <model.h>
#include <texture.h>

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
	boost::shared_ptr<Texture> texture;
	glm::mat4 modelMatrix;
	size_t mvpParam;
	size_t textureSampler;
};

#endif __GAME_H__
