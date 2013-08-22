#include <debug.h>
#include <textures/texture.h>
#include "tools.h"
#include <FreeImage.h>

//---------------------------------------------------------------

Texture::Texture(const std::string& filename)
	: filename(filename),
		minification(GL_LINEAR), magnification(GL_LINEAR),
		wrapModeS(GL_REPEAT), wrapModeT(GL_REPEAT), wrapModeR(GL_REPEAT)
{
	glGenTextures(1, &handle);
}

Texture::~Texture()
{
	glDeleteTextures(1, &handle);
}

void Texture::SetFiltering(GLenum minification, GLenum magnification)
{
	this->minification = minification;
	this->magnification = magnification;
}

GLenum Texture::GetMagnification() const { return magnification; }
GLenum Texture::GetMinification() const { return minification; }

