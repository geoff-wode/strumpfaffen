#if ! defined(__SHADER__)
#define __SHADER__

#include <map>
#include <string>
#include <glm/glm.hpp>
#include <gl_loader/gl_loader.h>
#include <boost/shared_ptr.hpp>

class ShaderUniform
{
public:
	virtual void Set(float value) = 0;
	virtual void Set(const glm::vec2& value) = 0;
	virtual void Set(const glm::vec3& value) = 0;
	virtual void Set(const glm::vec4& value) = 0;
	virtual void Set(const glm::mat2& value) = 0;
	virtual void Set(const glm::mat3& value) = 0;
	virtual void Set(const glm::mat4& value) = 0;

	virtual float GetFloat() const = 0;
	virtual glm::vec2 GetVec2() const = 0;
	virtual glm::vec3 GetVec3() const = 0;
	virtual glm::vec4 GetVec4() const = 0;
	virtual glm::mat2 GetMat2() const = 0;
	virtual glm::mat3 GetMat3() const = 0;
	virtual glm::mat4 GetMat4() const = 0;
};

typedef boost::shared_ptr<ShaderUniform> ShaderUniformPtr;
typedef std::map<std::string, ShaderUniformPtr> ShaderUniformMap;

// Shader semantics allow user code to correctly associate a vertex attribute
// to a shader input attribute 
namespace ShaderSemantics
{
	static const std::string Position    = "Position";
	static const std::string PositionLow = "PositionLow";		// Used when emulating double-precision maths on the GPU.
	static const std::string PositionHigh= "PositionHigh";	// Used when emulating double-precision maths on the GPU.
	static const std::string Colour			 = "Colour";
	static const std::string Normal			 = "Normal";
	static const std::string TexCoord0	 = "TexCoord0";
	static const std::string TexCoord1	 = "TexCoord1";
	static const std::string TexCoord2	 = "TexCoord2";
	static const std::string TexCoord3	 = "TexCoord3";
	static const std::string TexCoord4	 = "TexCoord4";
	static const std::string TexCoord5	 = "TexCoord5";
	static const std::string TexCoord6	 = "TexCoord6";
	static const std::string TexCoord7	 = "TexCoord7";
	static const std::string TexCoord8	 = "TexCoord8";
	static const std::string TexCoord9	 = "TexCoord9";
	static const std::string TexCoord10	 = "TexCoord10";
	static const std::string TexCoord11	 = "TexCoord11";
	static const std::string TexCoord12	 = "TexCoord12";
	static const std::string TexCoord13	 = "TexCoord13";
	static const std::string TexCoord14	 = "TexCoord14";
	static const std::string TexCoord15	 = "TexCoord15";
}

class Shader
{
public:
	Shader(const std::string& name);
	~Shader();

	bool Build();

	void Apply();

	const ShaderUniformMap& Uniforms;

private:
	const std::string& name;
	GLuint handle;
	ShaderUniformMap uniforms;
};

#endif // __SHADER__
