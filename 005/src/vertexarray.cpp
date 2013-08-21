#include <debug.h>
#include <vertexarray.h>

//----------------------------------------------------------------------------------

static struct AttribLocations
{
	const std::string semantic;
	const GLuint location;
} attribLocations[] =
{
	{ "VertexPosition"    , 0  },
	{ "VertexPositionLow" , 0  },
	{ "VertexPositionHigh", 1  },
	{ "VertexColour"      , 2  },
	{ "VertexNormal"      , 3  },
	{ "VertexTexCoord0"   , 4  },
	{ "VertexTexCoord1"   , 5  },
	{ "VertexTexCoord2"   , 6  },
	{ "VertexTexCoord3"   , 7  },
	{ "VertexTexCoord4"   , 8  },
	{ "VertexTexCoord5"   , 9  },
	{ "VertexTexCoord6"   , 10 },
	{ "VertexTexCoord7"   , 11 },
	{ "VertexTexCoord8"   , 12 },
	{ "VertexTexCoord9"   , 13 },
	{ "VertexTexCoord10"  , 14 },
	{ "VertexTexCoord11"  , 15 },
	{ "VertexTexCoord12"  , 16 },
	{ "VertexTexCoord13"  , 17 },
	{ "VertexTexCoord14"  , 18 },
	{ "VertexTexCoord15"  , 19 }
};
static const size_t NumAttribLocations = sizeof(attribLocations) / sizeof(attribLocations[0]);

//----------------------------------------------------------------------------------

static GLuint CreateVertexArray(
	const VertexArray::DeclarationList& declarations,
	boost::shared_ptr<IndexBuffer> indexBuffer);

//----------------------------------------------------------------------------------

VertexArray::VertexArray(const DeclarationList& declarations)
	: declarations(declarations), vao(CreateVertexArray(declarations, NULL))
{
}

VertexArray::VertexArray(const DeclarationList& declarations, boost::shared_ptr<IndexBuffer> indexBuffer)
	: declarations(declarations), indexBuffer(indexBuffer), vao(CreateVertexArray(declarations, indexBuffer))
{
}

VertexArray::VertexArray(const VertexDeclarationPtr* declarations, size_t declCount)
	: declarations(declarations, declarations + declCount)
{
	vao = CreateVertexArray(this->declarations, NULL);
}

VertexArray::VertexArray(const VertexDeclarationPtr* declarations, size_t declCount, boost::shared_ptr<IndexBuffer> indexBuffer)
	: declarations(declarations, declarations + declCount)
{
	vao = CreateVertexArray(this->declarations, NULL);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &vao);
}

void VertexArray::Enable()
{
	glBindVertexArray(vao);
}

//----------------------------------------------------------------------------------
static GLuint GetAttribLocation(const std::string& semantic)
{
	for (size_t i = 0; i < NumAttribLocations; ++i)
	{
		if (attribLocations[i].semantic == semantic) { return attribLocations[i].location; }
	}
	LOG("WARNING: unknown attribute semantic (%s) in shader\n", semantic);
	return 0;
}

//----------------------------------------------------------------------------------
static GLuint CreateVertexArray(const VertexArray::DeclarationList& declarations, boost::shared_ptr<IndexBuffer> indexBuffer)
{
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	if (indexBuffer) { indexBuffer->Enable(); }

	for (VertexArray::DeclarationList::const_iterator iter = declarations.cbegin(); iter != declarations.cend(); ++iter)
	{
		(*iter)->vertexBuffer->Enable();
		for (size_t i = 0; i < (*iter)->attributes.size(); ++i)
		{
			// The locations of attributes are fixed globally in the shader so they need to be found by attribute semantic...
			const GLuint attribIndex = GetAttribLocation((*iter)->attributes[i].semantic);

			// Now, enable and configure that particular location...
			glEnableVertexAttribArray(attribIndex);
			glVertexAttribPointer(attribIndex, (*iter)->attributes[i].size, (*iter)->attributes[i].type, GL_FALSE, (*iter)->stride, (const void*)(*iter)->attributes[i].offset);
		}
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (indexBuffer) { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

	return vao;
}
