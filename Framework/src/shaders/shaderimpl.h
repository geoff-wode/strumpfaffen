#if ! defined(__SHADER_IMPL_H__)
#define __SHADER_IMPL_H__

#include <glm/glm.hpp>
#include <gl_loader/gl_loader.h>
#include <shaders/shader.h>

using namespace Graphics;

class ShaderImpl : public IShader
{
public:
	ShaderImpl();
	virtual ~ShaderImpl();
	virtual const ShaderParamList& GetParams() const;
	virtual void Apply();

	GLuint handle;
	ShaderParamList params;
};

#endif // __SHADER_IMPL_H__
