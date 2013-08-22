#if ! defined(__CORE_BUFFERUSAGE__)
#define __CORE_BUFFERUSAGE__

namespace core
{
	namespace BufferUsage
	{
		enum Enum
		{
			Static,		// The buffer content never changes frame-to-frame.
			Dynamic		// The buffer content is likely to change frame-to-frame.
		};
	}
}

#endif // __CORE_BUFFERUSAGE__
