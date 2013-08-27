// The basis of a "scene".

#if ! defined(__SCENE__)
#define __SCENE__

#include <scenegraph/scenenode.h>
#include <scenegraph/matrixstack.h>
#include <boost/shared_ptr.hpp>

class Scene
{
public:
	Scene();
	~Scene();

	void LoadContent();
	void Update(unsigned int elapsedMS);
	void Render();

	MatrixStack matrixStack;
	SceneNodePtr rootNode;
};

typedef boost::shared_ptr<Scene> ScenePtr;

#endif // __SCENE__
