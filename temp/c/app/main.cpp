#include <SDL.h>
#include <stddef.h>
#include <glm/glm.hpp>
#include <vector>

#include <core/debug.h>
#include <core/program.h>
#include <input/keyboard.h>
#include <shaders/shader.h>
#include <geometry/mesh.h>

//-----------------------------------------------------------------

class MyProgram : public Core::Program
{
public:
	MyProgram()
	{
		Device->BackbufferWidth(1280);
		Device->BackbufferHeight(720);

		clearState.Colour = glm::vec4(0,0,1,1);
	}

	virtual ~MyProgram()
	{
	}

	virtual void Initialise()
	{
		Core::Program::Initialise();
		oldKeyState = Input::Keyboard::GetKeyState();

		shader = Shaders::LoadShader("simple");
		cubeMesh = Geometry::CreateMesh("models//cube.obj");
	}

	virtual void Update(unsigned int elapsedMS)
	{
		Input::KeyState keyState = Input::Keyboard::GetKeyState();

		if (keyState.IsKeyDown(SDLK_ESCAPE)) { Quit(); }

		oldKeyState = keyState;
		Core::Program::Update(elapsedMS);
	}

	virtual void Draw()
	{
		Device->Clear(clearState);

		shader->Activate();

		Core::Program::Draw();
	}

	Core::ClearState clearState;
	Input::KeyState oldKeyState;
	Shaders::Shader shader;
	Geometry::Mesh cubeMesh;
};

//-----------------------------------------------------------------

int main(int argc, char* argv[])
{
	MyProgram program;

	program.Run();

	return 0;
}
