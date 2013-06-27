#include <SDL.h>
#include <terrene/h/debug.h>
#include <terrene/h/program.h>
#include <terrene/h/inputmanager.h>
#include <terrene/h/shader.h>

struct VertexPositionTextureDecl
{
	VertexPositionTextureDecl(Terrene::Shader shader)
	{
		Attributes.push_back(shader->GetAttribute("position"));
		Attributes.push_back(shader->GetAttribute("texCoord"));
	}

	Terrene::ShaderAttributeCollection Attributes;
};


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

		VertexPositionTextureDecl vDecl(shader);
	}

	virtual void Update(float elapsedMS)
	{
		const Terrene::KeyboardState keyState(Terrene::InputManager::GetKeyboardState());
		if (keyState.IsKeyDown(SDLK_ESCAPE)) { Stop(); }
	}

	virtual void Draw(float elapsedMS)
	{
		Device->Clear(clearState);
	}

private:
	Terrene::KeyboardState prevKeyState;
	Terrene::ClearState		clearState;
	Terrene::Shader				shader;
};

int main(int argc, char* argv[])
{
	MyProgram program;

	program.Run();

	return 0;
}
