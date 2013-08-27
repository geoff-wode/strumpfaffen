#if ! defined(__SCENE__)
#define __SCENE__

#include <device.h>
#include <scenegraph/scenenode.h>
#include <scenegraph/matrixstack.h>

class Scene
{
public:
	SceneNodePtr	root;
	MatrixStack		matrixStack;
	Device&				device;

	Scene(Device& device);
	virtual ~Scene();

	void LoadContent();
	void Update(unsigned int elapsedMS);
	void Render();
};

#endif // __SCENE__
