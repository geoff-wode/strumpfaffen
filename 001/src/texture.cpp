#include <texture.h>
#include <debug.h>
#include <vector>
#include <string>
#include <SOIL.h>

//----------------------------------------------------------

Texture::Texture()
{
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}

boost::shared_ptr<Texture> Texture::New(GLint minFilter, GLint magFilter)
{
	boost::shared_ptr<Texture> texture(new Texture());
	if (texture)
	{
		glGenTextures(1, &texture->id);
		glBindTexture(GL_TEXTURE_2D, texture->id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	return texture;
}

bool Texture::Load(const std::string& filename)
{
	const std::string fullPath("images\\" + filename);

	return (0 != SOIL_load_OGL_texture(
		fullPath.c_str(), SOIL_LOAD_AUTO, id, SOIL_FLAG_DDS_LOAD_DIRECT));
}

void Texture::Apply() const
{
	glBindTexture(GL_TEXTURE_2D, id);
}
