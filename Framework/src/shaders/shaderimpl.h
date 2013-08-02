#if ! defined(__SHADER_IMPL_H__)
#define __SHADER_IMPL_H__

#include <vector>
#include <gl_loader/gl_loader.h>
#include <shaders/shader.h>
#include "shaderparamimpl.h"

using namespace Shaders;

class ShaderClassImpl : public ShaderClass
{
public:
	ShaderClassImpl(GLuint handle);
	virtual ~ShaderClassImpl();
	virtual ShaderParameter* const GetParameter(const std::string& name) const;
	virtual void Apply();

private:
	void GetParameters();

	GLuint handle;
	std::vector<ShaderParamImpl> parameters;
};


#endif // __SHADER_IMPL_H__
