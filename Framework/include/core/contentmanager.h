// Declares an interface to load content items (models, shaders, etc.).

#if ! defined(__CONTENT_MANAGER_H__)
#define __CONTENT_MANAGER_H__

#include <string>

namespace Content
{
	template <typename Interface> Interface Load(const std::string& name);
}

#endif // __CONTENT_MANAGER_H__
