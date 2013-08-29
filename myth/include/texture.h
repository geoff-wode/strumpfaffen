#if ! defined(__TEXTURE__)
#define __TEXTURE__

#include <string>
#include <gl_loader/gl_loader.h>
#include <boost/shared_ptr.hpp>

class Texture
{
public:
  Texture() { }
  ~Texture();

  bool Load(const std::string& name);

private:
  unsigned int handle;
  friend class Sampler;
};

class Sampler
{
public:
  Sampler(unsigned int slot) : slot(slot)
  {
    glGenSamplers(1, &handle);
  }
  ~Sampler()
  {
    glDeleteSamplers(1, &handle);
  }

  void SetMagFilter(GLenum value) { glSamplerParameteri(handle, GL_TEXTURE_MAG_FILTER, value); }
  void SetMinFilter(GLenum value) { glSamplerParameteri(handle, GL_TEXTURE_MIN_FILTER, value); }
  void SetWrapS(GLenum value) { glSamplerParameteri(handle, GL_TEXTURE_WRAP_S, value); }
  void SetWrapT(GLenum value) { glSamplerParameteri(handle, GL_TEXTURE_WRAP_T, value); }
  void SetTexture(boost::shared_ptr<Texture> texture) { this->texture = texture; }

  void Use()
  {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, texture->handle);
    glBindSampler(GL_TEXTURE0 + slot, handle);
  }

private:
  unsigned int slot;
  unsigned int handle;
  boost::shared_ptr<Texture> texture;
};

#endif // __TEXTURE__
