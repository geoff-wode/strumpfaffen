#include <texture.h>
#include <debug.h>
#include <vector>
#include <string>
#include <d3d9types.h>
#include <dds.h>

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
	bool loaded = false;

	const std::string fullPath("images\\" + filename);

	FILE* in = fopen(fullPath.c_str(), "rb");
	if (NULL != in)
	{
		char magic[4];
		if (sizeof(magic) == fread(magic, 1, sizeof(magic), in))
		{
			if (0 == strncmp("DDS ", magic, sizeof(magic)))
			{
				DDS_HEADER header;
				if (sizeof(header) == fread(&header, 1, sizeof(header), in))
				{
					unsigned int width = header.dwWidth;
					unsigned int height = header.dwHeight;
					const unsigned int mipMapCount = header.dwMipMapCount;
					const unsigned int linearSize = header.dwPitchOrLinearSize;
					const unsigned dataSize = (mipMapCount > 1) ? linearSize * 2 : linearSize;
					std::vector<unsigned char> data(dataSize);
					if (dataSize == fread(&data[0], 1, dataSize, in))
					{
						loaded = true;
						glBindTexture(GL_TEXTURE_2D, id);
						unsigned int format = (unsigned int)-1;
						switch (header.ddspf.dwFourCC)
						{
						case MAKEFOURCC('D','X','T','1'): format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; break;
						case MAKEFOURCC('D','X','T','3'): format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT; break;
						case MAKEFOURCC('D','X','T','5'): format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT; break;
						default:
							loaded = false;
						}

						if (loaded)
						{
							const unsigned int blockSize = (GL_COMPRESSED_RGBA_S3TC_DXT1_EXT == format) ? 8 : 16;
							unsigned int offset;
							for (unsigned int level = 0; (level < mipMapCount) && (width || height); ++level)
							{
								unsigned int size = ((header.dwWidth + 3) / 4) * ((header.dwHeight + 3) / 4) * blockSize;
								glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, &data[offset]);
								offset += size;
								width /= 2;
								height /= 2;
							}
						}

						glBindTexture(GL_TEXTURE_2D, 0);
					}
				}
			}
		}
		fclose(in);
	}

	return loaded;
}

void Texture::Apply() const
{
	glBindTexture(GL_TEXTURE_2D, id);
}
