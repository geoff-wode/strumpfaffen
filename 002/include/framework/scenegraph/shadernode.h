#if ! defined(__SCENEGRAPH_SHADERNODE_H__)
#define __SCENEGRAPH_SHADERNODE_H__

#include <string>
#include <framework/scenegraph/scenenode.h>
#include <framework/shaders/shader.h>

namespace Framework
{
	class ShaderNode : public SceneNode
	{
	public:
		ShaderNode(const std::string& name);
		virtual ~ShaderNode();

		virtual void LoadResources(Scene* const scene);

		virtual bool PreRender(Scene* const scene);
		virtual void Render(Scene* const scene);
		virtual void PostRender(Scene* const scene);

	protected:
		std::string name;
		ShaderPtr shader;
	};
}

#endif // __SCENEGRAPH_SHADERNODE_H__
