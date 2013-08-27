#include "shadersemantic.h"
#include <map>
#include <string>

typedef std::map<std::string, ShaderSemantic> ShaderSemanticMap;

const ShaderSemantic* const ShaderSemantic::Get(const std::string& semantic)
{
	static const ShaderSemantic defaultValue;
	static ShaderSemanticMap shaderSemantics;
	static bool initialised = false;

	if (!initialised)
	{
		shaderSemantics["Position"]			= ShaderSemantic(0, GL_FLOAT, 3, false);
		shaderSemantics["PositionLow"]	= ShaderSemantic(0, GL_FLOAT, 3, false);
		shaderSemantics["PositionHigh"] = ShaderSemantic(1, GL_FLOAT, 3, false);
		shaderSemantics["Normal"]				= ShaderSemantic(2, GL_FLOAT, 3, false);
		shaderSemantics["Colour"]				= ShaderSemantic(3, GL_FLOAT, 4, false);
		shaderSemantics["Texture0"]			= ShaderSemantic(4, GL_FLOAT, 4, false);
		shaderSemantics["Texture1"]			= ShaderSemantic(5, GL_FLOAT, 4, false);
		shaderSemantics["Texture2"]			= ShaderSemantic(6, GL_FLOAT, 4, false);
		shaderSemantics["Texture3"]			= ShaderSemantic(7, GL_FLOAT, 4, false);
		initialised = true;
	}

	const ShaderSemanticMap::const_iterator it = shaderSemantics.find(semantic);
	if (it != shaderSemantics.cend())
	{
		return &it->second;
	}

	return NULL;
}
