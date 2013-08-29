#if ! defined(__SHADER__)
#define __SHADER__

#include <string>
#include <boost/noncopyable.hpp>
#include <gl_loader/gl_loader.h>

class Shader : public boost::noncopyable
{
public:
  Shader(const std::string& name);
  ~Shader();

  void Use();

  // Return the index for attribute 'name' in the shader.
  GLint GetAttributeIndex(const std::string& name);

private:
  const GLuint programHandle;
};

#endif // __SHADER__
