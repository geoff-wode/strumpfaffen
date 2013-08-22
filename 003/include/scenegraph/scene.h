#if ! defined(__SCENE__)
#define __SCENE__

#include <device.h>
#include <scenegraph/scenenode.h>
#include <scenegraph/matrixstack.h>

class Scene
{
public:
	Scene(boost::shared_ptr<Device> device);
	~Scene();

	void Load();
	void Update();
	void Render();

	boost::shared_ptr<SceneNode> root;
	boost::shared_ptr<Device> device;
	MatrixStack matrixStack;
};

#endif // __SCENE__
