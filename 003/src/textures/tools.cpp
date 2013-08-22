#include <gl_loader/gl_loader.h>
#include <debug.h>
#include "tools.h"

//------------------------------------------------------------------------

boost::shared_ptr<FIBITMAP> TextureImpl::TextureLoadImage(const std::string& filename)
{
	// Deduce file format...
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filename.c_str());
	if (FIF_UNKNOWN == fif)
	{
		fif = FreeImage_GetFIFFromFilename(filename.c_str());
		if (FIF_UNKNOWN == fif)
		{
			// Cannot determine the format of the file - give up!
			return NULL;
		}
	}

	// Can we actually read the file in..?
	if (!FreeImage_FIFSupportsReading(fif))
	{
		// Erm...No.
		return NULL;
	}

	// Load the file...
	boost::shared_ptr<FIBITMAP> img(FreeImage_Load(fif, filename.c_str()), FreeImage_Unload);

	return img;
}
