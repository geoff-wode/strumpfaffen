#include <core/debug.h>
#include <buffers/vertexbuffer.h>
#include <gl_loader/gl_loader.h>
#include <boost/make_shared.hpp>

//---------------------------------------------------------------------------------------------

class VertexBufferImpl : public VertexBuffer
{
public:
	VertexBufferImpl(size_t size, GLenum usage)
		: size(size)
	{
		// Create the buffer...
		glGenBuffers(1, &handle);
		Enable();
		glBufferData(GL_ARRAY_BUFFER, size, NULL, usage);		
		Disable();
	}

	virtual ~VertexBufferImpl()
	{
		glDeleteBuffers(1, &handle);
	}

	virtual size_t GetSize() const { return size; }

	virtual void SetData(const void* const data, size_t size, size_t offset)
	{
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	}

	virtual void GetData(size_t size, size_t offset, void* const data)
	{
		glGetBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	}

	virtual void Enable()
	{
		glBindBuffer(GL_ARRAY_BUFFER, handle);
	}

	virtual void Disable()
	{
		glBindVertexArray(0);
	}

	size_t size;
	GLuint handle;
};

//---------------------------------------------------------------------------------------------

boost::shared_ptr<VertexBuffer> CreateVertexBuffer
	(
		size_t size,
		BufferUsageHint::Enum usage
	)
{
	GLenum glUsage = 0;

	switch (usage)
	{
	case BufferUsageHint::Dynamic: glUsage = GL_DYNAMIC_DRAW; break;
	case BufferUsageHint::Static: glUsage = GL_STATIC_DRAW; break;
	default: return NULL;
	}

	return boost::make_shared<VertexBufferImpl>(size, glUsage);
}
