#include <SDL.h>
#include <framework/scenegraph/scene.h>

using namespace Framework;

//--------------------------------------------------------------

Scene::Scene(Device* const device)
	: device(device), lastTime(0)
{
	shaderSceneGlobals.ViewMatrix = glm::mat4(1);
	shaderSceneGlobals.ProjectionMatrix = glm::mat4(1);
	shaderSceneGlobals.ViewProjectionMatrix = glm::mat4(1);
	shaderSceneGlobals.CameraPosition = glm::vec4(0);

	glGenBuffers(1, &shaderSceneGlobalsBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, shaderSceneGlobalsBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(ShaderSceneGlobals), &shaderSceneGlobals, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, shaderSceneGlobalsBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	device->uniformBufferBindings["CommonShaderVarsBlock"] = device->uniformBufferBindings.size();
}

Scene::~Scene()
{
}

void Scene::LoadResources()
{
	if (root) { root->LoadResources(this); }
}

void Scene::Update()
{
	const unsigned int now = SDL_GetTicks();
	const unsigned int elapsedMS = now - lastTime;
	lastTime = now;

	if (movementController) { movementController->Update(elapsedMS); }
	if (root) { root->Update(this, elapsedMS); }
}

void Scene::Render()
{
	shaderSceneGlobals.ViewProjectionMatrix = shaderSceneGlobals.ViewMatrix * shaderSceneGlobals.ProjectionMatrix;
	glBindBuffer(GL_UNIFORM_BUFFER, shaderSceneGlobalsBuffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(ShaderSceneGlobals), &shaderSceneGlobals, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	if (root && root->PreRender(this))
	{
		root->Render(this);
		root->RenderChildren(this);
		root->PostRender(this);
	}
}
