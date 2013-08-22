// Provides a (strong) hint as to how a buffer will be used and accessed.

#if ! defined(__BUFFERS_BUFFERHINT__)
#define __BUFFERS_BUFFERHINT__

namespace BufferUsageHint
{
	enum Enum
	{
		Dynamic,
		Static
	};
}

#endif // __BUFFERS_BUFFERHINT__
