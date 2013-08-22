#if ! defined(__SHADER_COMPILER_DEFINES__)
#define __SHADER_COMPILER_DEFINES__

// These definitions are compiled into every shader...
static const char* ShaderAutos =
		"#define POSITION				layout(location = 0)\n"
		"#define POSITION_LOW		layout(location = 0) // Used when emulating double-precision floats\n"
		"#define POSITION_HIGH  layout(location = 1) // Used when emulating double-precision floats\n"
		"#define NORMAL					layout(location = 2)\n"
		"#define COLOUR					layout(location = 3)\n"
		"#define TEXCOORD0			layout(location = 4)\n"
		"#define TEXCOORD1			layout(location = 5)\n"
		"#define TEXCOORD2			layout(location = 6)\n"
		"#define TEXCOORD3			layout(location = 7)\n"
		"#define TEXCOORD4			layout(location = 8)\n"
		"#define TEXCOORD5			layout(location = 9)\n"
		"#define TEXCOORD6			layout(location = 11)\n"
		"#define TEXCOORD7			layout(location = 12)\n"
		"#define TEXCOORD8			layout(location = 13)\n"
		"#define TEXCOORD9			layout(location = 14)\n"
		"#define TEXCOORD10			layout(location = 15)\n"
		"#define TEXCOORD12			layout(location = 16)\n"
		"#define TEXCOORD13			layout(location = 17)\n"
		"#define TEXCOORD14			layout(location = 18)\n"
		"#define TEXCOORD15			layout(location = 19)\n"
		"\n"
		"#define PI							3.1415927f\n"
		"#define TWO_PI					(PI * 2.0f)\n"
		"#define PI_OVER_2			(PI * 0.5f)\n"
		"\n"
		"layout (std140) uniform SharedUniformsBlock\n"
		"{\n"
		"  uniform mat4 WorldMatrix;\n"
		"  uniform mat4 InverseWorldMatrix;\n"
		"  uniform mat4 ViewMatrix;\n"
		"  uniform mat4 ProjectionMatrix;\n"
		"  uniform mat4 ViewProjectionMatrix;\n"
		"  uniform mat4 WorldViewProjectionMatrix;\n"
		"  uniform vec4 CameraPos;\n"
		"};\n"
		"\n"
	;

// These names are used to determine the location of the corresponding attribute in the shader...
static const char* AttribName[] =
{
	// "Position" is a specially-recognised case
	"Position_Low",
	"Position_High",
	"Normal",
	"Colour",
	"TexCoord0",
	"TexCoord1",
	"TexCoord2",
	"TexCoord3",
	"TexCoord4",
	"TexCoord5",
	"TexCoord6",
	"TexCoord7",
	"TexCoord8",
	"TexCoord9",
	"TexCoord10",
	"TexCoord12",
	"TexCoord13",
	"TexCoord14",
	"TexCoord15",
	NULL
};

#endif // __SHADER_COMPILER_DEFINES__
