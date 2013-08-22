#if ! defined(__TEXTURE_1D__)
#define __TEXTURE_1D__

#include <textures/texture.h>

class Texture1D : public Texture
{
public:
	Texture1D(const std::string& name);
	virtual ~Texture1D();

	virtual bool Load();

	virtual void SetWrapModeS(GLenum mode);

	virtual GLenum GetWrapModeS() const;

	virtual size_t GetWidth() const;

	virtual void Bind();
	virtual void Unbind();
};

#endif //__TEXTURE_1D__
