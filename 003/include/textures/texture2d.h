#if ! defined(__TEXTURE_2D__)
#define __TEXTURE_2D__

#include <textures/texture.h>

class Texture2D : public Texture
{
public:
	Texture2D(const std::string& name);
	virtual ~Texture2D();

	virtual bool Load();
				
	virtual void SetWrapModeS(GLenum mode);
	virtual void SetWrapModeT(GLenum mode);

	virtual GLenum GetWrapModeS() const;
	virtual GLenum GetWrapModeT() const;

	virtual size_t GetWidth() const;
	virtual size_t GetHeight() const;

	virtual void Bind();
	virtual void Unbind();
};

#endif //__TEXTURE_2D__
