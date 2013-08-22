#include <textures/texture1d.h>
#include "tools.h"

//------------------------------------------------------------

Texture1D::Texture1D(const std::string& filename)
	: Texture(filename)
{
}

Texture1D::~Texture1D()
{
}

void Texture1D::SetWrapModeS(GLenum mode) { wrapModeS = mode; }

GLenum Texture1D::GetWrapModeS() const { return wrapModeS; }

size_t Texture1D::GetWidth() const { return width; }

void Texture1D::Bind() { glBindTexture(GL_TEXTURE_1D, handle); }
void Texture1D::Unbind() { glBindTexture(GL_TEXTURE_1D, 0); }

bool Texture1D::Load()
{
	// Load the bitmap.
	// The smart pointer ensures that the image is properly unloaded on scope exit...
	boost::shared_ptr<FIBITMAP> img(TextureImpl::TextureLoadImage(filename));

	if (img)
	{
		bpp = FreeImage_GetBPP(img.get());
		width = FreeImage_GetWidth(img.get());
		const GLenum imgFormat = (bpp == 24) ? GL_BGR : (bpp == 8) ? GL_R8 : 0;
		const GLint internalFormat = (bpp == 24) ? GL_RGB : (bpp == 8) ? GL_R8 : 0;

		Bind();
		glTexImage1D(GL_TEXTURE_1D, 0, internalFormat, width, 0, imgFormat, GL_UNSIGNED_BYTE, img.get());
		glGenerateMipmap(GL_TEXTURE_1D);
		Unbind();
	}

	return img;
}
