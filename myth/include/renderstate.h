#if ! defined(__RENDER_STATE__)
#define __RENDER_STATE__

#include <glm/glm.hpp>
#include <textureunit.h>
#include <gl_loader/gl_loader.h>
#include <boost/shared_ptr.hpp>

// Describes a complete rendering state.
struct RenderState
{
  RenderState()
    : colourMask(true), depthMask(true),
      depthTestEnabled(true), depthFunc(GL_LESS),
      enableCulling(true), cullFace(GL_BACK), frontFace(GL_CCW)
  {
  }

  glm::bvec4 colourMask;
  bool depthMask;
  bool depthTestEnabled;
  GLenum depthFunc;

  bool enableCulling;
  GLenum cullFace;
  GLenum frontFace;

  GLuint vao;
  boost::shared_ptr<Shader> shader;

  static const size_t MaxTextureUnits = 16;
  TextureUnit textureUnits[MaxTextureUnits];
};

#endif // __RENDER_STATE__
