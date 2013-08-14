#if ! defined(__BUFFER_H__)
#define __BUFFER_H__

#include <boost/shared_ptr.hpp>

namespace Graphics
{
	namespace BufferUsageHint
	{
		enum Enum
		{
			Static,
			Dynamic,
			Stream
		};
	}

	namespace BufferType
	{
		enum Enum
		{
			Vertex,
			Index,
			ShaderUniform
		};
	}

	struct IBuffer
	{
		virtual void SetData(const void* const data, size_t size, size_t offset) = 0;
		virtual void GetData(void* const data, size_t size, size_t offset) const = 0;
		virtual void Enable() const = 0;
		virtual void Disable() const = 0;
		virtual size_t GetSize() const = 0;
		virtual unsigned int GetHandle() const = 0;
	};
	typedef boost::shared_ptr<IBuffer> Buffer;

	Buffer CreateBuffer(BufferType::Enum type, size_t size, BufferUsageHint::Enum usage);
}

#endif // __BUFFER_H__
