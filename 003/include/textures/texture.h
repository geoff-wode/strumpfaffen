#if ! defined(__TEXTURE__)
#define __TEXTURE__

#include <string>
#include <gl_loader/gl_loader.h>

class Texture
{
public:
	virtual ~Texture();

	virtual bool Load() = 0;

	void SetFiltering(GLenum minification, GLenum magnification);
	GLenum GetMagnification() const;
	GLenum GetMinification() const;

	size_t GetBPP() const;

	virtual void SetWrapModeS(GLenum mode) = 0;
	virtual void SetWrapModeT(GLenum mode) = 0;
	virtual void SetWrapModeR(GLenum mode) = 0;

	virtual GLenum GetWrapModeS() const = 0;
	virtual GLenum GetWrapModeT() const = 0;
	virtual GLenum GetWrapModeR() const = 0;

	virtual size_t GetWidth() const = 0;
	virtual size_t GetHeight() const = 0;
	virtual size_t GetDepth() const = 0;

	virtual void Bind() = 0;
	virtual void Unbind() = 0;

protected:
	Texture(const std::string& filename);

	const std::string filename;
	GLuint handle;
	GLenum minification;
	GLenum magnification;
	size_t bpp;
	size_t width;
	size_t height;
	size_t depth;
	GLenum wrapModeS;
	GLenum wrapModeT;
	GLenum wrapModeR;
};

#endif // __TEXTURE__
