#if ! defined(__INDEX_BUFFER_H__)
#define __INDEX_BUFFER_H__

#include <boost/shared_ptr.hpp>
#include <buffers/bufferusagehint.h>

namespace Buffers
{
	// Define the data types which can be stored in an index buffer.
	namespace IndexBufferType
	{
		enum Enum
		{
			Byte = 1,
			Short = 2,
			Int = 4
		};
	}

	class IIndexBuffer
	{
	public:
		virtual void SetData(const void* const data, size_t count, size_t startIndex) const = 0;

		virtual void GetData(void* const data, size_t count, size_t startIndex) const = 0;

		virtual void Activate() const = 0;
		virtual void Deactivate() const = 0;

		virtual size_t ElementCount() const = 0;

		virtual IndexBufferType::Enum ElementType() const = 0;
	};

	typedef boost::shared_ptr<IIndexBuffer> IndexBuffer;

	IndexBuffer CreateIndexBuffer(IndexBufferType::Enum dataType, size_t count, UsageHint::Enum usageHint);

}

#endif // __INDEX_BUFFER_H__
