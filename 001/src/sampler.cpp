#include <debug.h>
#include <sampler.h>
#include <texture.h>
#include <boost/scoped_ptr.hpp>

//-----------------------------------------------------------------------

Sampler::Sampler(GLenum type)
		: type(type),
			BoundTexture(NULL),
			MagFilter(GL_LINEAR), MinFilter(GL_LINEAR_MIPMAP_LINEAR),
			WrapS(GL_REPEAT), WrapT(GL_REPEAT), WrapR(GL_REPEAT)
{
}

Sampler::~Sampler()
{
}

boost::shared_ptr<Sampler> Sampler::New(GLenum type)
{
	boost::shared_ptr<Sampler> sampler(new Sampler(type));

	return sampler;
}

void Sampler::Apply() const
{
	if (BoundTexture)
	{
		glBindTexture(type, BoundTexture->GetID());
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, MagFilter);
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, MinFilter);

		switch (type)
		{
		case GL_SAMPLER_3D: glTexParameteri(type, GL_TEXTURE_WRAP_R, WrapR);
		case GL_SAMPLER_2D: glTexParameteri(type, GL_TEXTURE_WRAP_T, WrapT);
		case GL_SAMPLER_1D: glTexParameteri(type, GL_TEXTURE_WRAP_S, WrapS);
			break;
		default:
			ASSERTM(false, "invalid sampler type");
		}
	}
}

bool Sampler::BindTexture(boost::shared_ptr<Texture> texture, GLuint unit)
{
	bool bound = false;

	switch (type)
	{
	case GL_SAMPLER_1D: glBindTexture(GL_TEXTURE_1D, texture->GetID()); bound = true; break;
	case GL_SAMPLER_2D: glBindTexture(GL_TEXTURE_2D, texture->GetID()); bound = true; break;
	case GL_SAMPLER_3D: glBindTexture(GL_TEXTURE_3D, texture->GetID()); bound = true; break;
	default: ASSERTM(false, "sample and texture types not compatible");
	}

	if (bound)
	{
		this->unit = unit;
		BoundTexture = texture;
		glActiveTexture(GL_TEXTURE0 + unit);
	}

	return bound;
}

void Sampler::UnbindTexture()
{
	switch (type)
	{
	case GL_SAMPLER_1D: glBindTexture(GL_TEXTURE_1D, 0); break;
	case GL_SAMPLER_2D: glBindTexture(GL_TEXTURE_2D, 0); break;
	case GL_SAMPLER_3D: glBindTexture(GL_TEXTURE_3D, 0); break;
	default: ASSERTM(false, "sample and texture types not compatible");
	}

	BoundTexture = NULL;
}
