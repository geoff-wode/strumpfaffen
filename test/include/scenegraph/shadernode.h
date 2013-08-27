#if ! defined(__SHADER_NODE__)
#define __SHADER_NODE__

#include <string>
#include <shader.h>
#include <scenegraph/scenenode.h>

class ShaderNode : public SceneNode
{
public:
	ShaderNode(const std::string& shaderName);

	virtual void LoadContent(Scene* const scene);

	virtual void Render(Scene* const scene);

private:
	const std::string shaderName;
	Shader	shader;
};

#endif // __SHADER_NODE__
