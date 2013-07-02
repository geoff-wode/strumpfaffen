#include <SDL.h>
#include <terrene/h/debug.h>
#include <terrene/h/program.h>
#include <terrene/h/inputmanager.h>
#include <terrene/h/shader/shader.h>
#include <terrene/h/model.h>
#include <glm/gtc/matrix_transform.hpp>

class MyProgram : public Terrene::Program
{
public:
	MyProgram()
	{
		Device->BackbufferWidth = 1280;
		Device->BackbufferHeight = 900;
	}

	virtual ~MyProgram()
	{
	}

protected:
	virtual void Initialise()
	{
		prevKeyState = Terrene::InputManager::GetKeyboardState();

		clearState.ClearColour = Terrene::Colour::Blue;

		shader = Terrene::LoadShader("simple");

		const glm::mat4 world(glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0.0f)));
		const glm::mat4 view(glm::lookAt(glm::vec3(0,0,-200), glm::vec3(0,0,0), glm::vec3(0,1,0)));
		const glm::mat4 project(glm::perspective(45.0f, Device->GetViewport().AspectRatio, 0.1f, 1000.0f));
		const glm::mat4 mvp = project * view * world;

		Terrene::ShaderParameter	mvpParam = shader->GetParameter("ModelViewProjection");
		mvpParam->Set(mvp);

		model = Terrene::LoadModel("untitled.obj");
	}

	virtual void Update(float elapsedMS)
	{
		const Terrene::KeyboardState keyState(Terrene::InputManager::GetKeyboardState());
		if (keyState.IsKeyDown(SDLK_ESCAPE)) { Stop(); }
	}

	virtual void Draw(float elapsedMS)
	{
		Device->Clear(clearState);
		shader->Activate();
		model->Render();
	}

private:
	Terrene::KeyboardState prevKeyState;
	Terrene::ClearState		clearState;
	Terrene::Shader				shader;
	Terrene::Model				model;
};

int main(int argc, char* argv[])
{
	MyProgram program;

	program.Run();

	return 0;
}
