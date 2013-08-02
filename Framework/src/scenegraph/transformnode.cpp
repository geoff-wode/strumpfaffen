#include <scenegraph/transformnode.h>
#include <scenegraph/scene.h>

using namespace SceneGraph;

//------------------------------------------------------------------------------------

TransformNodeClass::TransformNodeClass()
	: ToWorld(1), FromWorld(1)
{
}

TransformNodeClass::~TransformNodeClass()
{
}

bool TransformNodeClass::PreRender(Scene* const scene)
{
	if (!scene->MatrixStack.empty())
	{
		scene->MatrixStack.push(scene->MatrixStack.top() * ToWorld);
	}
	else
	{
		scene->MatrixStack.push(ToWorld);
	}
	return SceneNodeClass::PreRender(scene);
}

void TransformNodeClass::PostRender(Scene* const scene)
{
	if (!scene->MatrixStack.empty())
	{
		scene->MatrixStack.pop();
	}
}

void TransformNodeClass::SetTransform(const glm::mat4& toWorld)
{
	SetTransform(toWorld, glm::inverse(toWorld));
}

void TransformNodeClass::SetTransform(const glm::mat4& toWorld, const glm::mat4& fromWorld)
{
	ToWorld = toWorld;
	FromWorld = fromWorld;
}
