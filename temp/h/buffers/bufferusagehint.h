#if ! defined(__BUFFER_USAGE_HINT_H__)
#define __BUFFER_USAGE_HINT_H__

namespace Buffers
{

	namespace UsageHint
	{
		enum Enum
		{
			Static,			// data defined once and used repeatedly
			Dynamic,		// data defined every few frames and used once
			Streaming		// data defined every frame and used once
		};
	}

}

#endif // __BUFFER_USAGE_HINT_H__
