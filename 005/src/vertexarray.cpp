#include <debug.h>
#include <vertexarray.h>
#include "shadersemantics.h"

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
	for (size_t i = 0; i < NumShaderSemantics; ++i)
	{
		if (shaderSemantics[i] == semantic) { return (i > 0) ? (i - 1) : i; }
	}
	LOG("WARNING: unknown attribute semantic (%s) in shader\n", semantic.c_str());
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
			LOG(
				"vertex array: %s - %d x %s @ %d\n",
				(*iter)->attributes[i].semantic.c_str(),
				(*iter)->attributes[i].size,
				GetAttribTypeName((*iter)->attributes[i].type),
				attribIndex
				);
			glEnableVertexAttribArray(attribIndex);
			glVertexAttribPointer(attribIndex, (*iter)->attributes[i].size, (*iter)->attributes[i].type, GL_FALSE, (*iter)->stride, (const void*)(*iter)->attributes[i].offset);
		}
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (indexBuffer) { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

	return vao;
}
