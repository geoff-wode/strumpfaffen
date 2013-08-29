#if ! defined(__SCENE__)
#define __SCENE__

#include <device.h>
#include <scenegraph/scenenode.h>
#include <scenegraph/matrixstack.h>
#include <scenegraph/cameranode.h>

class Scene
{
public:
	SceneNodePtr	root;
	MatrixStack		matrixStack;
	Device&				device;

  boost::shared_ptr<CameraNode> camera;

	// The scene accumulates render state from each node as it traverses the graph.
	// Nodes should modify the state in Render().
	RenderState		renderState;

	Scene(Device& device);
	virtual ~Scene();

	void LoadContent();
	void Update();
	void Render();

  void Quit() { quit = true; }
	bool HasQuit() { return quit; }

private:
	bool quit;
	float prevTime;
};

#endif // __SCENE__
