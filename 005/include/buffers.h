#if ! defined(__BUFFERS__)
#define __BUFFERS__

#include <gl_loader/gl_loader.h>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

class Buffer
{
public:
	virtual ~Buffer();

	void SetData(const void* const data, size_t size, size_t offset);
	void GetData(void* const data, size_t size, size_t offset);
	void Enable();

protected:
	Buffer(GLenum type, size_t size, GLenum usage);

private:
	GLenum type;
	GLuint handle;
};

//----------------------------------------------------------------------------------

class IndexBuffer : public Buffer
{
public:
	IndexBuffer(size_t size, GLenum usage) : Buffer(GL_ELEMENT_ARRAY_BUFFER, size, usage)
	{	}

	static void Disable() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
};

class VertexBuffer : public Buffer
{
public:
	VertexBuffer(size_t size, GLenum usage) : Buffer(GL_ARRAY_BUFFER, size, usage)
	{	}

	static void Disable() { glBindBuffer(GL_ARRAY_BUFFER, 0); }
};

class UniformBuffer : public Buffer
{
public:
	UniformBuffer(size_t size, GLenum usage) : Buffer(GL_UNIFORM_BUFFER, size, usage)
	{	}

	static void Disable() { glBindBuffer(GL_UNIFORM_BUFFER, 0); }
};

//----------------------------------------------------------------------------------

boost::shared_ptr<IndexBuffer> CreateIndexBuffer(size_t size, GLenum usage);
boost::shared_ptr<VertexBuffer> CreateVertexBuffer(size_t size, GLenum usage);
boost::shared_ptr<UniformBuffer> CreateUniformBuffer(size_t size, GLenum usage);

#endif // __BUFFERS__
