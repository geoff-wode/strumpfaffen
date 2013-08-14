#include <scenegraph/scene.h>
#include <scenegraph/scenenode.h>
#include <shaders/uniformblock.h>
#include <buffers/buffer.h>

using namespace SceneGraph;
using namespace Graphics;

//-----------------------------------------------------------------------

static unsigned int nextBlockBufferBinding = 0;

//-----------------------------------------------------------------------

boost::shared_ptr<Scene> SceneGraph::Scene::Create(Core::Device* const device)
{
	boost::shared_ptr<Scene> scene(new Scene(device));

	ShaderUniformBlock block;
	
	block.name = "CommonShaderVarsBlock";
	block.binding = nextBlockBufferBinding++;
	block.buffer = CreateBuffer(BufferType::ShaderUniform, sizeof(CommonShaderVarsBlock), BufferUsageHint::Dynamic);

	block.buffer->Enable();
	glBindBufferRange(GL_UNIFORM_BUFFER, block.binding, block.buffer->GetHandle(), 0, sizeof(CommonShaderVarsBlock));
	block.buffer->Disable();

	scene->UniformBlocks.push_back(block);

	return scene;
}

Scene::Scene(Core::Device* device)
	: Device(device)
{
}

void SceneGraph::LoadResources(Scene* const scene)
{
	if (scene->Root) { scene->Root->LoadResources(scene); }
}

void SceneGraph::Update(Scene* const scene, unsigned int elapsedMS)
{
	if (scene->Root) { scene->Root->Update(scene, elapsedMS); }
}

void SceneGraph::Render(Scene* const scene)
{
	if (scene->Root && scene->Root->PreRender(scene))
	{
		scene->Root->Render(scene);
		scene->Root->RenderChildren(scene);
		scene->Root->PostRender(scene);
	}
}
