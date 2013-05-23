#if ! defined(__SHADERS_H__)
#define __SHADERS_H__

#include <string>
#include <gl/gl_loader.h>

bool NewShaderProgram(const std::string& vsSrc, const std::string& fsSrc, GLuint* program);

#endif // __SHADERS_H__
