#if ! defined(__TEXTURE_TOOLS__)
#define __TEXTURE_TOOLS__

#include <string>
#include <FreeImage.h>
#include <boost/shared_ptr.hpp>

namespace TextureImpl
{
	boost::shared_ptr<FIBITMAP> TextureLoadImage(const std::string& filename);
}

#endif // __TEXTURE_TOOLS__
