#include <SDL.h>

#include <util/debug.h>
#include <core/program.h>
#include <input/keyboard.h>
#include <geometry/model.h>

//-----------------------------------------------------------------

class TheProgram : public Core::Program
{
public:
	TheProgram()
	{
		Device->BackbufferWidth(1280);
		Device->BackbufferHeight(720);
	}

	virtual ~TheProgram()
	{
	}

protected:
	virtual void Initialise()
	{
		oldKeyState = Input::Keyboard::GetKeyState();
		clearState.Colour = glm::vec4(0,0,1,1);
	}

	virtual void LoadResources()
	{
		monkey = Geometry::LoadModel("models\\monkey.obj");
	}

	virtual void Update(unsigned int elapsedMS)
	{
		Input::KeyState keyState = Input::Keyboard::GetKeyState();
		if (keyState.IsKeyDown(SDLK_ESCAPE))
		{
			Quit();
		}

		oldKeyState = keyState;
	}

	virtual void Draw()
	{
		Device->Clear(clearState);
		monkey->Render();
	}

private:
	Input::KeyState oldKeyState;
	Core::ClearState clearState;
	Geometry::Model monkey;
};

//-----------------------------------------------------------------

int main(int argc, char* argv[])
{
	TheProgram program;
	program.Run();
	return 0;
}
