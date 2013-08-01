#if ! defined(__SHADER_X__)
#define __SHADER_X__

#include <string>
#include <map>
#include <boost/shared_ptr.hpp>
#include <terrene/h/shaderX/vertexattribute.h>
#include <terrene/h/shaderX/parameter.h>

namespace Terrene
{
	namespace Shaders
	{

		typedef std::map<std::string,VertexAttribute> AttributeList;

		class IShader
		{
		public:
			// Return a mappable list of vertex attributes.
			virtual const AttributeList& VertexAttributes() const = 0;

			// Return the framebuffer index of the given fragment output variable 'fragVar'.
			// Will return -1 if the variable does not exist in the shader's fragment stage.
			virtual const unsigned int FragmentOutput(const std::string& fragVar) const = 0;

			// Return a direct reference to a named shader parameter.
			virtual Parameter GetParameter(const std::string& name) const = 0;

			virtual void Activate() = 0;

		protected:
			~IShader() { }
		};

		typedef boost::shared_ptr<IShader> Shader;
		Shader LoadShader(const std::string& name);
	}
}

#endif // __SHADER_X__
