#if ! defined(__CORE_DEVICE__)
#define __CORE_DEVICE__

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <render/clearstate.h>
#include <render/renderstate.h>

namespace core
{
	class Device : public boost::noncopyable
	{
	public:
		Device();
		~Device();

		std::string WindowTitle;
		int BackbufferWidth;
		int BackbufferHeight;

		bool Initialise();
		void SwapBuffers();

		void Clear
			(
				const ClearState& clearState,
				ClearState::Buffers::Enum buffers
			);

		void Draw
			(
				const RenderState& renderState
			);

	private:
		struct Impl;
		boost::shared_ptr<Impl> impl;
	};
}

#endif // __CORE_DEVICE__
