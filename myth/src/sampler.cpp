#include <gl_loader/gl_loader.h>
#include <sampler.h>

//-----------------------------------------------------

Sampler2D::Sampler2D(unsigned int slot)
  : slot(slot)
{
  glGenSamplers(1, &handle);
  glSamplerParameteri(handle, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glSamplerParameteri(handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glSamplerParameteri(handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glSamplerParameteri(handle, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Sampler2D::~Sampler2D()
{
  glDeleteSamplers(1, &handle);
}

//-----------------------------------------------------

void Sampler2D::SetMagFilter(GLenum value) { glSamplerParameteri(handle, GL_TEXTURE_MAG_FILTER, value); }
void Sampler2D::SetMinFilter(GLenum value) { glSamplerParameteri(handle, GL_TEXTURE_MIN_FILTER, value); }
void Sampler2D::SetWrapS(GLenum value) { glSamplerParameteri(handle, GL_TEXTURE_WRAP_S, value); }
void Sampler2D::SetWrapT(GLenum value) { glSamplerParameteri(handle, GL_TEXTURE_WRAP_T, value); }

void Sampler2D::SetTexture(boost::shared_ptr<Texture2D> texture)
{
  this->texture = texture;
}

void Sampler2D::Activate()
{
  glActiveTexture(GL_TEXTURE0 + slot);
  texture->Activate();
  glBindSampler(GL_TEXTURE0 + slot, handle);
}

void Sampler2D::Deactivate()
{
  glActiveTexture(GL_TEXTURE0 + slot);
  Texture2D::Deactivate();
  glBindSampler(GL_TEXTURE0 + slot, handle);
}
