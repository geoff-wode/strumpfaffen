#include <textures/texture2d.h>
#include "tools.h"

//------------------------------------------------------------

Texture2D::Texture2D(const std::string& filename)
	: Texture(filename)
{
}

Texture2D::~Texture2D()
{
}

void Texture2D::SetWrapModeS(GLenum mode) { wrapModeS = mode; }
void Texture2D::SetWrapModeT(GLenum mode) { wrapModeT = mode; }

GLenum Texture2D::GetWrapModeS() const { return wrapModeS; }
GLenum Texture2D::GetWrapModeT() const { return wrapModeT; }

size_t Texture2D::GetWidth() const { return width; }
size_t Texture2D::GetHeight() const { return height; }

void Texture2D::Bind() { glBindTexture(GL_TEXTURE_2D, handle); }
void Texture2D::Unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

bool Texture2D::Load()
{
	// Load the bitmap.
	// The smart pointer ensures that the image is properly unloaded on scope exit...
	boost::shared_ptr<FIBITMAP> img(TextureImpl::TextureLoadImage(filename));

	if (img)
	{
		bpp = FreeImage_GetBPP(img.get());
		width = FreeImage_GetWidth(img.get());
		height = FreeImage_GetHeight(img.get());
		const GLenum imgFormat = (bpp == 24) ? GL_BGR : (bpp == 8) ? GL_R8 : 0;
		const GLint internalFormat = (bpp == 24) ? GL_RGB : (bpp == 8) ? GL_R8 : 0;

		Bind();
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, imgFormat, GL_UNSIGNED_BYTE, img.get());
		glGenerateMipmap(GL_TEXTURE_2D);
		Unbind();
	}

	return img;
}
