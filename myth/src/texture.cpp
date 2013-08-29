#include <SOIL.h>
#include <texture.h>
#include <gl_loader/gl_loader.h>

//--------------------------------------------------

Texture::~Texture()
{
  glDeleteTextures(1, &handle);
}

//--------------------------------------------------

bool Texture::Load(const std::string& name)
{
  handle = SOIL_load_OGL_texture(
    name.c_str(),
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);

  if (0 != handle)
  {
    glBindTexture(GL_TEXTURE_2D, handle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  return (0 != handle);
}
