#if ! defined(__BUFFERS_H__)
#define __BUFFERS_H__

#include <gl/gl_loader.h>
#include <boost/shared_ptr.hpp>

class StaticBuffer
{
public:
	static boost::shared_ptr<StaticBuffer> New();

	virtual ~StaticBuffer();

	void SetData(const void* const data, size_t dataSize);

	void Apply() const;

private:
	StaticBuffer();
	GLuint id;
};

#endif // __BUFFERS_H__
