#if ! defined(__BUFFER__)
#define __BUFFER__

#include <stddef.h>
#include <gl_loader/gl_loader.h>

class Buffer
{
public:
	Buffer(size_t size, GLenum type, GLenum usage);
	virtual ~Buffer();

	void SetData(const void* const data, size_t size, size_t offset);
	void GetData(void* const data, size_t size, size_t offset);

	void Bind();
	void Unbind();

	GLuint GetHandle() const;

private:
	GLenum type;
	GLuint handle;
};

#endif // __BUFFER__
