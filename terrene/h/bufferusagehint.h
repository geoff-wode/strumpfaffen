#if ! defined(__BUFFER_USAGE_HINT_H__)
#define __BUFFER_USAGE_HINT_H__

namespace Terrene
{

	namespace BufferUsageHint
	{
		enum Enum
		{
			Static,		// modified once, used every frame
			Stream,		// modified often, used for one frame
			Dynamic		// (partially) modified often, used every frame
		};
	}

}

#endif // __BUFFER_USAGE_HINT_H__
