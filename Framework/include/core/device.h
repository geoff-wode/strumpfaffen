#if ! defined(__DEVICE_H__)
#define __DEVICE_H__

#include <boost/scoped_ptr.hpp>
#include <core/clearstate.h>
#include <core/viewport.h>

namespace Core
{

	class Device
	{
	public:
		Device();
		~Device();

		void BackbufferWidth(unsigned int value);
		void BackbufferHeight(unsigned int value);
		unsigned int BackbufferWidth() const;
		unsigned int BackbufferHeight() const;

		bool Initialise();

		void Clear();													// use default clear state
		void Clear(const ClearState& state);	// use a specific clear state

		static unsigned int MaxVertexAttributes();

		Viewport	GetViewport() const;
		void SetViewport(const Viewport& value);

		glm::mat4 ViewMatrix;
		glm::mat4 ProjectionMatrix;

	private:
		struct DeviceImpl;
		boost::scoped_ptr<DeviceImpl> impl;
	};

}

#endif // __DEVICE_H__
