#if ! defined(__BUFFERS__)
#define __BUFFERS__

#include <boost/shared_ptr.hpp>
#include <gl_loader/gl_loader.h>

class BufferClass
{
public:
	const GLenum type;
	const size_t size;
	const GLenum usage;

	virtual ~BufferClass()
	{
		glDeleteBuffers(1, &handle);
	}

	void Enable() { glBindBuffer(type, handle); }

	void SetData(const void* const data, size_t size, size_t offset)
	{
		Enable();
		glBufferSubData(type, offset, size, data);
		glBindBuffer(type, 0);
	}

protected:
	BufferClass(GLenum type, size_t size, GLenum usage)
		: type(type), size(size), usage(usage)
	{
		glGenBuffers(1, &handle);
		glBindBuffer(type, handle);
		glBufferData(type, size, NULL, usage);
	}

	GLuint handle;
};

//--------------------------------------------------------------------------------------------
class VertexBufferClass : public BufferClass
{
public:
	VertexBufferClass(size_t size, GLenum usage)
		: BufferClass(GL_ARRAY_BUFFER, size, usage)
	{
		Disable();
	}

	static void Disable() { glBindBuffer(GL_ARRAY_BUFFER, 0); }
};

//--------------------------------------------------------------------------------------------
class IndexBufferClass : public BufferClass
{
public:
	IndexBufferClass(size_t size, GLenum usage, GLenum type)
		: BufferClass(GL_ELEMENT_ARRAY_BUFFER, size, usage),
			indexType(type)
	{
		Disable();
	}

	static void Disable() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
	
	const GLenum indexType;
};

//--------------------------------------------------------------------------------------------
class UniformBufferClass : public BufferClass
{
public:
	UniformBufferClass(size_t size, GLuint bindingPoint)
		: BufferClass(GL_UNIFORM_BUFFER, size, GL_DYNAMIC_DRAW)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, handle);
		Disable();
	}

	static void Disable() { glBindBuffer(GL_UNIFORM_BUFFER, 0); }
};

//--------------------------------------------------------------------------------------------

typedef boost::shared_ptr<VertexBufferClass> VertexBuffer;
typedef boost::shared_ptr<IndexBufferClass> IndexBuffer;
typedef boost::shared_ptr<UniformBufferClass> UniformBuffer;

#endif //__BUFFERS__
