#if ! defined(__CORE_UNIFORMBUFFER__)
#define __CORE_UNIFORMBUFFER__

#include <core/bufferusage.h>
#include <boost/scoped_ptr.hpp>

namespace core
{
	class UniformBuffer
	{
	public:
		// Create a uniform buffer.
		// size				the number of bytes the buffer should occupy
		// usage			a hint as to how the buffer will be used
		UniformBuffer(size_t size, BufferUsage::Enum usage, unsigned int bindPoint);
		~UniformBuffer();

		const size_t Size;
		const BufferUsage::Enum Usage;

		// Make the current buffer "active".
		void Activate();

		// Make the index buffer inactive.
		static void Deactivate();

		// Copy size bytes of data into the buffer, starting at offset.
		void Set(const void* const data, size_t size, size_t offset);

		// Copy size bytes, starting at offset from the buffer into the data area.
		void Get(size_t size, size_t offset, void* const data);

	private:
		struct Impl;
		boost::scoped_ptr<Impl> impl;
	};
}

#endif // __CORE_UNIFORMBUFFER__
