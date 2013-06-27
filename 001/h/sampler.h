#if ! defined(__SAMPLER_H__)
#define __SAMPLER_H__

#include <vector>
#include <gl/gl_loader.h>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <texture.h>

class Sampler : public boost::noncopyable
{
public:
	static boost::shared_ptr<Sampler> New(GLenum type);

	virtual ~Sampler();

	boost::shared_ptr<Texture> BoundTexture;
	GLint MagFilter;
	GLint MinFilter;
	GLint	WrapS;
	GLint	WrapT;
	GLint	WrapR;

	// Binds the given texture to specififed unit in the shader sampler.
	// Returns false if the dimensionality of the texture and sampler do not match.
	bool BindTexture(boost::shared_ptr<Texture> texture, GLuint unit);

	void UnbindTexture();

	void Apply() const;

protected:
	Sampler(GLenum type);

	const GLenum type;
	GLuint unit;
};

#endif // __SAMPLER_H__
