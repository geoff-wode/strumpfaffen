#if ! defined(__BUFFER_H__)
#define __BUFFER_H__

#include <stddef.h>

namespace Terrene
{
	class IBuffer
	{
	public:
		// Copy 'sizeInBytes' system data into the buffer from 'data' (assumed to be large enough!).
		virtual void SetData(const void* const data, size_t sizeInBytes) = 0;

		// Copy 'sizeInBytes' system data into the buffer at 'offset' from 'data' (assumed to be large enough!).
		virtual void SetData(const void* const data, size_t sizeInBytes, size_t offset) = 0;

		// Copy 'sizeInBytes' from the buffer into 'data' (assumed to be large enough!).
		virtual void GetData(size_t sizeInBytes, void* const data) = 0;

		// Copy 'sizeInBytes' from the buffer starting at 'offset' into 'data' (assumed to be large enough!).
		virtual void GetData(size_t sizeInBytes, size_t offset, void* const data) = 0;

		// Make this buffer type active.
		virtual void Activate() const = 0;

	protected:
		~IBuffer() { }
	};
}

#endif // __BUFFER_H__
