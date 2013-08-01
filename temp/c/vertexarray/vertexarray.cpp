// A vertex array contains information on how vertex data is laid out in memory.
// The vertex array is static during runtime, so the configuration is stored in a
// Vertex Array Object to avoid the cost of enabling the attribute(s) during rendering.

#include <boost/scoped_array.hpp>
#include <core/debug.h>
#include <core/device.h>
#include <vertexarray/vertexarray.h>
#include <glm/gtc/half_float.hpp>
#include <gl_loader/gl_loader.h>

using namespace VertexArrays;
using namespace Buffers;

//-----------------------------------------------------------------

class VertexArrayImpl : public IVertexArray
{
public:
	VertexArrayImpl(VertexBuffer vb, IndexBuffer ib)
		: vb(vb), ib(ib)
	{
		// Create vertex array object that contains the vertex layout,  vertex and optional index buffers
		// in its internal state...
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		{
			vb->Activate();
			if (ib) { ib->Activate(); }

			Framework::VertexDeclaration vertexDecl = vb->VertexDeclaration();
			Framework::VertexElementArray elements = vertexDecl.GetElements();

			for (size_t i = 0; i < vertexDecl.GetElementCount(); ++i)
			{
				glEnableVertexAttribArray(i);
				glVertexAttribPointer(
					i,
					elements[i].Components(),
					elements[i].Type,
					GL_FALSE,
					vertexDecl.VertexStride(),
					(const void*)elements[i].Offset);
			}
		}
		glBindVertexArray(0);

		// Make sure the buffers are not active globally...
		vb->Deactivate();
		if (ib) { ib->Deactivate(); }
	}

	~VertexArrayImpl()
	{
		glDeleteVertexArrays(1, &vao);
	}

	virtual const Framework::VertexDeclaration& GetVertexDeclaration() const
	{
		return vb->VertexDeclaration();
	}

	virtual VertexBuffer GetVertexBuffer() const
	{
		return vb;
	}

	virtual IndexBuffer GetIndexBuffer() const
	{
		return ib;
	}

	virtual void Activate() const
	{
		glBindVertexArray(vao);
	}

	virtual void Deactivate() const
	{
		glBindVertexArray(0);
	}

	GLuint vao;
	const Buffers::VertexBuffer vb;
	const Buffers::IndexBuffer ib;
};

//-----------------------------------------------------------------

VertexArray VertexArrays::CreateVertexArray(VertexBuffer vb)
{
	VertexArray va(new VertexArrayImpl(vb, NULL));
	return va;
}

VertexArray VertexArrays::CreateVertexArray(VertexBuffer vb, IndexBuffer ib)
{
	VertexArray va(new VertexArrayImpl(vb, ib));
	return va;
}
