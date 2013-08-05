#include <SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <util/debug.h>
#include <core/program.h>
#include <input/keyboard.h>
#include <geometry/mesh.h>
#include <scenegraph/scene.h>
#include <scenegraph/camera.h>
#include <scenegraph/meshnode.h>
#include <scenegraph/shadernode.h>
#include <scenegraph/transformnode.h>

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

		Device->ProjectionMatrix = glm::perspective(45.0f, Device->GetViewport().GetAspectRatio(), 0.1f, 1000.0f);

		scene.Device = Device.get();

		SceneGraph::TransformNode rootNode(new SceneGraph::TransformNodeClass());
		SceneGraph::CameraNode cameraNode(new SceneGraph::CameraNodeClass());

		SceneGraph::TransformNode monkeyNode(new SceneGraph::TransformNodeClass());
		SceneGraph::ShaderNode monkeyShader(new SceneGraph::ShaderNodeClass("shaders/simplest"));
		SceneGraph::MeshNode monkeyMesh(new SceneGraph::MeshNodeClass("models/monkey.obj"));
		monkeyNode->Children.push_back(monkeyShader);
		monkeyNode->Children.push_back(monkeyMesh);
		monkeyNode->SetTransform(glm::translate(glm::mat4(1), glm::vec3(0,0,-20)));

		scene.Root = rootNode;
		scene.Root->Children.push_back(cameraNode);
		scene.Root->Children.push_back(monkeyNode);		
	}

	virtual void LoadResources()
	{
		scene.LoadResources();
	}

	virtual void Update(unsigned int elapsedMS)
	{
		Input::KeyState keyState = Input::Keyboard::GetKeyState();
		if (keyState.IsKeyDown(SDLK_ESCAPE))
		{
			Quit();
		}

		scene.Update(elapsedMS);

		oldKeyState = keyState;
	}

	virtual void Draw()
	{
		Device->Clear(clearState);
		scene.Render();
	}

private:
	Input::KeyState oldKeyState;
	Core::ClearState clearState;
	SceneGraph::Scene scene;
};

//-----------------------------------------------------------------

int main(int argc, char* argv[])
{
	TheProgram program;
	program.Run();
	return 0;
}
