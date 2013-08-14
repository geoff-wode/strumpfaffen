#if ! defined(__DRAW_STATE__)
#define __DRAW_STATE__

#include <boost/shared_ptr.hpp>
#include <geometry/vertexdeclaration.h>
#include <shaders/shaderprogram.h>

struct DrawState
{
	boost::shared_ptr<ShaderProgram> shader;
	boost::shared_ptr<VertexDeclaration> vertexDecl;
};

#endif // __DRAW_STATE__
