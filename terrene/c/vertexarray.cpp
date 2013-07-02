#include <vertexarray.h>
#include <vertextypes.h>
#include <debug.h>
#include <list>
#include <gl_loader/gl_loader.h>

using namespace Terrene;

//------------------------------------------------------------------------------

static size_t ComputeStride(unsigned int numElements, const VertexArrayElement* const elements)
{
	unsigned int stride = 0;
	for (unsigned int i = 0; i < numElements; ++i)
	{
		stride += elements[i].Size;
	}
	return stride;
}

//------------------------------------------------------------------------------

VertexDeclaration::VertexDeclaration(unsigned int numElements, const VertexArrayElement* const elements)
	: Stride(ComputeStride(numElements, elements)),
		NumElements(numElements),
		Elements(elements)
{
}

//------------------------------------------------------------------------------

VertexArrayElement::VertexArrayElement(const std::string& name, GLenum type, size_t components, size_t size, size_t offset)
	: Name(name), Type(type), Components(components), Size(size), Offset(offset)
{
}

//------------------------------------------------------------------------------

class VertexArrayImpl : public IVertexArray
{
public:
	VertexArrayImpl(const VertexDeclaration& vertexDecl)
		: vertexDecl(vertexDecl)
	{
	}

	virtual void Activate()
	{
		if (ib != Indices)
		{
			ib = Indices;
			if (ib)
			{
				ib->Activate();
			}
			else
			{
				IIndexBuffer::Deactivate();
			}
		}

		if (vb != Vertices)
		{
			vb = Vertices;
			if (vb)
			{
				vb->Activate();
				for (unsigned int i = 0; i < vertexDecl.NumElements; ++i)
				{
					glEnableVertexAttribArray(i);
					glVertexAttribPointer(i,
						vertexDecl.Elements[i].Components,
						GL_FLOAT,
						GL_FALSE,
						vertexDecl.Stride,
						(const void*)vertexDecl.Elements[i].Offset);
				}
			}
			else
			{
				IVertexBuffer::Deactivate();
			}
		}
	}

	VertexBuffer vb;
	IndexBuffer	ib;
	VertexDeclaration vertexDecl;
};

//------------------------------------------------------------------------------

VertexArray	Terrene::CreateVertexArray(const VertexDeclaration& vertexDeclaration)
{
	VertexArray va(new VertexArrayImpl(vertexDeclaration));

	return va;
}
