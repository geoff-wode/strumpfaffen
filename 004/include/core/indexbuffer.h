#if ! defined(__CORE_INDEXBUFFER__)
#define __CORE_INDEXBUFFER__

#include <core/bufferusage.h>
#include <boost/scoped_ptr.hpp>

namespace core
{
	namespace IndexBufferDataType
	{
		enum Enum
		{
			UnsignedShort,
			UnsignedInt
		};
	}

	class IndexBuffer
	{
	public:
		// Create an index buffer.
		// size				the number of bytes the buffer should occupy
		// usage			a hint as to how the buffer will be used
		// dataType		defines which data type is used as an index value.
		IndexBuffer(size_t size, BufferUsage::Enum usage, IndexBufferDataType::Enum dataType);

		const size_t Size;
		const BufferUsage::Enum Usage;
		const IndexBufferDataType::Enum DataType;

		// Make the current buffer "active".
		void Activate();

		// Make the index buffer inactive.
		static void Deactivate();

		// Copy size bytes of data into the buffer, starting at offset.
		void Set(const void* const data, size_t size, size_t offset);

		// Copy size bytes, starting at offset from the buffer into the data area.
		void Get(size_t size, size_t offset, void* const data);

	protected:
		~IndexBuffer();

	private:
		struct Impl;
		boost::scoped_ptr<Impl> impl;
	};
}

#endif // __CORE_INDEXBUFFER__
