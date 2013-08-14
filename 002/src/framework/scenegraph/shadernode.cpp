#include <framework/scenegraph/shadernode.h>
#include <framework/scenegraph/scene.h>

using namespace Framework;

//---------------------------------------------------------------------------

ShaderNode::ShaderNode(const std::string& name)
	: name(name)
{
}

ShaderNode::~ShaderNode()
{
}

void ShaderNode::LoadResources(Scene* const scene)
{
	shader = scene->device->CreateShader(name);
}

bool ShaderNode::PreRender(Scene* const scene)
{
	if (!shader) { LoadResources(scene); }

	if (shader) { shader->Enable(); }

	return shader;
}

void ShaderNode::Render(Scene* const scene)
{
	shader->Apply();
}

void ShaderNode::PostRender(Scene* const scene)
{
	shader->Disable();
}
