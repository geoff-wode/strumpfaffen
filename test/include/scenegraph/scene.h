#if ! defined(__SCENE__)
#define __SCENE__

#include <device.h>
#include <movementcontroller.h>
#include <scenegraph/scenenode.h>
#include <scenegraph/matrixstack.h>

class Scene
{
public:
	SceneNodePtr	root;
	MatrixStack		matrixStack;
	Device&				device;
	boost::shared_ptr<MovementController> movementController;

	// The scene accumulates render state from each node as it traverses the graph.
	// Nodes should modify the state in Render().
	RenderState		renderState;

	Scene(Device& device);
	virtual ~Scene();

	void LoadContent();
	void Update(unsigned int elapsedMS);
	void Render();
};

#endif // __SCENE__
