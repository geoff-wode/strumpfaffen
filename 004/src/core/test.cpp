#include <stdlib.h>
#include <string>
#include <map>
#include <stdio.h>
#include <vector>
#include <gl_loader/gl_loader.h>
#include <glm/glm.hpp>
#include <debug.h>

//-----------------------------------------------------------------------------

static GLuint CompileShader(const char* source, GLenum type);

//-----------------------------------------------------------------------------

typedef std::map<std::string,unsigned int> VertexAttributeIndicesMap;
static VertexAttributeIndicesMap VertexAttributeIndices;

static std::string ShaderDefinitions
	(
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
	);

static void InitVertexAttributeIndices()
{
	unsigned int index = 0;
	VertexAttributeIndices["Position"] = 0;
	VertexAttributeIndices["PositionHigh"] = index++;
	VertexAttributeIndices["PositionLow"] = index++;
	VertexAttributeIndices["Normal"] = index++;
	VertexAttributeIndices["Colour"] = index++;

	char line[128];
	for (int i = 0; i < 16; ++i)
	{
		sprintf(line, "TexCoord%d", i);
		VertexAttributeIndices[line] = index;
	}
}

static const char* VertexShader =
		"#version 330\n"
		"\n"
		"POSITION in vec3 position;\n"
		"COLOUR in vec3 colour;\n"
		"\n"
		"out vec3 theColour;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(position,1);\n"
		"	theColour = colour;\n"
		"}\n"
		"\n";

static const char* FragmentShader =
		"#version 330\n"
		"in vec3 theColour;\n"
		"out vec4 fragmentColour;\n"
		"void main()\n"
		"{\n"
		"  fragmentColour = vec4(theColour,1);\n"
		"}\n"
		"\n";

//-----------------------------------------------------------------------------
class VertexBuffer
{
public:
	VertexBuffer(size_t size, GLenum usage)
	{
		glGenBuffers(1, &handle);
		Enable();
		glBufferData(GL_ARRAY_BUFFER, size, NULL, usage);
		Disable();
	}

	~VertexBuffer() { glDeleteBuffers(1, &handle); }

	void SetData(const void* const data, size_t size, size_t offset)
	{
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	}

	void Enable()
	{
		if (handle != boundBuffer)
		{
			glBindBuffer(GL_ARRAY_BUFFER, handle);
			boundBuffer = handle;
		}
	}

	static void Disable()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		boundBuffer = 0;
	}

private:
	GLuint handle;
	static GLuint boundBuffer;
};
GLuint VertexBuffer::boundBuffer = 0;

//-----------------------------------------------------------------------------
struct VertexAttribute
{
	VertexAttribute(const std::string& name, GLenum type, GLuint numComponents, GLuint offset)
		: name(name), type(type), numComponents(numComponents), offset(offset)
	{
	}

	std::string name;
	GLenum type;
	GLuint numComponents;
	GLuint offset;
};

struct VertexAttributeArray
{
	size_t stride;
	VertexBuffer* vertexBuffer;
	std::vector<VertexAttribute*> attributes;
};

class VertexDeclaration
{
public:
	VertexDeclaration(const VertexAttributeArray* const attrArrays, size_t numArrays)
		: vertexArrays(attrArrays, attrArrays + numArrays)
	{
		glGenVertexArrays(1, &handle);
		Enable();
		for (size_t i = 0; i < vertexArrays.size(); ++i)
		{
			vertexArrays[i].vertexBuffer->Enable();
			for (size_t a = 0; a < vertexArrays[i].attributes.size(); ++a)
			{
				VertexAttributeIndicesMap::const_iterator iter = VertexAttributeIndices.find(vertexArrays[i].attributes[a]->name);
				if (VertexAttributeIndices.cend() != iter)
				{
					glEnableVertexAttribArray(iter->second);
					glVertexAttribPointer(iter->second,
						vertexArrays[i].attributes[a]->numComponents,
						vertexArrays[i].attributes[a]->type,
						GL_FALSE,
						vertexArrays[i].stride,
						(const void*)vertexArrays[i].attributes[a]->offset);
				}
			}
		}
		Disable();
	}

	~VertexDeclaration()
	{
		glDeleteVertexArrays(1, &handle);
	}

	void Enable()
	{
		if (handle != boundVAO)
		{
			glBindVertexArray(handle);
			boundVAO = handle;
		}
	}

	static void Disable()
	{
		glBindVertexArray(0);
		boundVAO = 0;
	}

private:
	std::vector<VertexAttributeArray> vertexArrays;
	GLuint handle;
	static GLuint boundVAO;
};
GLuint VertexDeclaration::boundVAO = 0;

//-----------------------------------------------------------------------------
void test()
{
	static const float points[] =
	{
		 0.0f,  0.5f,  0.0f,
		 0.5f, -0.5f,  0.0f,
		-0.5f, -0.5f,  0.0f
	};
	static const float colours[] = {
		1.0f, 0.0f,  0.0f,
		0.0f, 1.0f,  0.0f,
		0.0f, 0.0f,  1.0f
	};

	InitVertexAttributeIndices();

	VertexAttributeArray va[2];

	{
		va[0].vertexBuffer = new VertexBuffer(sizeof(points), GL_STATIC_DRAW);
		va[0].vertexBuffer->Enable();
		va[0].vertexBuffer->SetData(points, sizeof(points), 0);
		va[0].attributes.push_back(new VertexAttribute("Position", GL_FLOAT, 3, 0));

		va[1].vertexBuffer = new VertexBuffer(sizeof(colours), GL_STATIC_DRAW);
		va[1].vertexBuffer->Enable();
		va[1].vertexBuffer->SetData(points, sizeof(colours), 0);
		va[1].attributes.push_back(new VertexAttribute("Colour", GL_FLOAT, 3, 0));
		VertexBuffer::Disable();
	}

	const GLuint program = glCreateProgram();
	glAttachShader(program, CompileShader(VertexShader, GL_VERTEX_SHADER));
	glAttachShader(program, CompileShader(FragmentShader, GL_FRAGMENT_SHADER));
	glLinkProgram(program);
	GLint linkedOK;
	glGetProgramiv(program, GL_LINK_STATUS, &linkedOK);
	if (GL_TRUE != linkedOK)
	{
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<GLchar> log(logLength+1);
		glGetProgramInfoLog(program, logLength, NULL, log.data());
		LOG("%s\n", log.data());
		exit(EXIT_FAILURE);
	}

	VertexDeclaration decl(va, 2);

	exit(EXIT_SUCCESS);
}

static GLuint CompileShader(const char* source, GLenum type)
{
	const GLchar* sources[2] = { ShaderDefinitions.c_str(), source };

	const GLuint shader = glCreateShader(type);
	glShaderSource(shader, 2, sources, NULL);
	glCompileShader(shader);

	GLint OK;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &OK);
	if (GL_TRUE != OK)
	{
		GLint logLength;
		glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		std::vector<GLchar> log(logLength+1);
		glGetProgramInfoLog(shader, logLength, NULL, log.data());
		LOG("%s\n", log.data());
	}
	return shader;
}
