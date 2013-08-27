#include <scenegraph/shadernode.h>
#include <scenegraph/scene.h>

//---------------------------------------------------

ShaderNode::ShaderNode(const std::string& shaderName)
	: shaderName(shaderName),
		shader(new ShaderClass(shaderName))
{
}

//---------------------------------------------------

void ShaderNode::LoadContent(Scene* const scene)
{
	shader->Build();
}

void ShaderNode::Render(Scene* const scene)
{
	scene->renderState.shader = shader;
}
