#if ! defined(__SHADER_ATTRIBUTE_H__)
#define __SHADER_ATTRIBUTE_H__

#include <list>

namespace Terrene
{

	class SShaderAttribute
	{
	public:
		const std::string Name;
		const GLenum			Type;
		const int					Location;
		const size_t			ComponentCount;

		SShaderAttribute(const char* const name, GLenum type, int location, size_t componentCount)
			: Name(name), Type(type), Location(location), ComponentCount(componentCount)
		{
		}
	};

	typedef SShaderAttribute* ShaderAttribute;
	typedef std::list<ShaderAttribute> ShaderAttributeCollection;
}

#endif // __SHADER_ATTRIBUTE_H__
