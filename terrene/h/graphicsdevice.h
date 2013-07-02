#if ! defined(__GRAPHICS_DEVICE_H__)
#define __GRAPHICS_DEVICE_H__

#include <boost/noncopyable.hpp>
#include <terrene/h/viewport.h>
#include <terrene/h/clearstate.h>
#include <terrene/h/buffers/vertexbuffer.h>

namespace Terrene
{

	class GraphicsDevice : public boost::noncopyable
	{
	public:
		GraphicsDevice();

		bool Initialise();

		void SetViewport(const Viewport& viewport);
		Viewport GetViewport() const;

		void Clear(const ClearState& state);

		bool FullScreen;
		int BackbufferWidth;
		int BackbufferHeight;

	private:
		Viewport currentViewport;
		ClearState currentClearState;
	};

}

#endif // __GRAPHICS_DEVICE_H__
