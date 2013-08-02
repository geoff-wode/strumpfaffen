#include <scenegraph/scene.h>
#include <scenegraph/shadernode.h>
#include <shaders/shader.h>

using namespace SceneGraph;

//--------------------------------------------------------------------------------

ShaderNodeClass::ShaderNodeClass(const std::string& filename)
	: filename(filename)
{
}

ShaderNodeClass::~ShaderNodeClass()
{
}

void ShaderNodeClass::LoadResources(Scene* const scene)
{
	shader = Shaders::ShaderClass::Load(filename);
}

bool ShaderNodeClass::PreRender(Scene* const scene)
{
	if (shader)
	{
		shader->Apply();
	}
	return true;
}

Shaders::Shader ShaderNodeClass::GetShader() const
{
	return shader;
}
