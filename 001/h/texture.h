#if ! defined(__TEXTURE_H__)
#define __TEXTURE_H__

#include <gl/gl_loader.h>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

class Texture : public boost::noncopyable
{
public:
	static boost::shared_ptr<Texture> New(GLenum type);

	virtual ~Texture();
	bool Load(const std::string& filename);

	GLuint GetID() const { return id; }

protected:
	Texture(GLenum type);
	const GLenum type;
	GLuint id;
};

//class Texture
//{
//public:
//	static boost::shared_ptr<Texture> New(size_t instance);
//	
//	virtual ~Texture();
//
//	bool Load(const std::string& filename);
//
//	void Apply() const;
//
//	GLint	magFilter;
//	GLint minFilter;
//	GLint wrapS;
//	GLint wrapT;
//
//private:
//	Texture();
//	GLuint id;
//	GLuint instance;
//	bool isActive;
//};

#endif // __TEXTURE_H__
