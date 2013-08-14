#if ! defined(__SHADER_ATTRIBUTE__)
#define __SHADER_ATTRIBUTE__

#include <map>
#include <string>
#include <gl_loader/gl_loader.h>

struct ShaderAttribute
{
	GLenum type;
	GLuint size;			// the count of 'type' elements in the attribute (e.g. vec2: type=float, size=2)
	GLuint location;
};

typedef std::map<std::string, ShaderAttribute> ShaderAttributeMap;

#endif // __SHADER_ATTRIBUTE__
