#if ! defined(__SAMPLER__)
#define __SAMPLER__

#include <texture.h>
#include <boost/shared_ptr.hpp>

class Sampler2D
{
public:
  Sampler2D(unsigned int slot);
  ~Sampler2D();

  void SetMagFilter(GLenum value);
  void SetMinFilter(GLenum value);
  void SetWrapS(GLenum value);
  void SetWrapT(GLenum value);
  void SetTexture(boost::shared_ptr<Texture2D> texture);

  void Activate();
  void Deactivate();

private:
  unsigned int handle;
  unsigned int slot;
  boost::shared_ptr<Texture2D> texture;
};

#endif // __SAMPLER__
