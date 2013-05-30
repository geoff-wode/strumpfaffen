#if ! defined(__TEXTURE_H__)
#define __TEXTURE_H__

#include <gl/gl_loader.h>
#include <string>
#include <boost/shared_ptr.hpp>

class Texture
{
public:
	static boost::shared_ptr<Texture> New();
	
	virtual ~Texture();

	bool Load(const std::string& filename);

	void Apply() const;

	GLint	magFilter;
	GLint minFilter;
	GLint wrapS;
	GLint wrapT;

private:
	Texture();

	GLuint id;
};

#endif // __TEXTURE_H__
