#include <debug.h>
#include <texture.h>
#include <SOIL.h>

boost::shared_ptr<Texture> Texture::New(GLenum type)
{
	boost::shared_ptr<Texture> texture(new Texture(type));

	glGenTextures(1, &texture->id);

	return texture;
}

Texture::Texture(GLenum type)
	: type(type), id(0)
{
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}

bool Texture::Load(const std::string& filename)
{
	const std::string fullPath("images\\" + filename);

	return (0 != SOIL_load_OGL_texture(
		fullPath.c_str(), SOIL_LOAD_AUTO, id, SOIL_FLAG_DDS_LOAD_DIRECT));
}
