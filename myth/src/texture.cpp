#include <SOIL.h>
#include <texture.h>
#include <gl_loader/gl_loader.h>
#include <boost/scoped_ptr.hpp>

//--------------------------------------------------

Texture2D::Texture2D(const std::string& name)
  : name(name),
    width(0), height(0)
{
  glGenTextures(1, &handle);
}

Texture2D::~Texture2D()
{
  glDeleteTextures(1, &handle);
}

//--------------------------------------------------

bool Texture2D::Load()
{
  // Load the image data from file...
  int channels;
  unsigned char* img = SOIL_load_image(name.c_str(), (int*)&width, (int*)&height, &channels, SOIL_LOAD_AUTO);

  // Upload the image into the texture...
  handle = SOIL_create_OGL_texture(
    img, width, height, channels, handle,
    SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);

  SOIL_free_image_data(img);

  return (0 != handle);
}

//--------------------------------------------------
void Texture2D::Activate() const
{
  glBindTexture(GL_TEXTURE_2D, handle);
}

void Texture2D::Deactivate()
{
  glBindTexture(GL_TEXTURE_2D, 0);
}
