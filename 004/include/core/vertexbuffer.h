#if ! defined(__CORE_VERTEXBUFFER__)
#define __CORE_VERTEXBUFFER__

#include <map>
#include <string>
#include <core/bufferusage.h>
#include <core/attributetype.h>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

namespace core
{
	struct VertexAttribute
	{
		std::string name;
		AttributeType::Enum type;
		size_t offset;
	};

	struct VertexDeclaration
	{
		const size_t Stride;
		const size_t NumAttributes;
		const VertexAttribute* const Attributes;
	};

	class VertexBuffer
	{
	public:
		// Create a vertex buffer.
		// size				the number of bytes the buffer should occupy
		// usage			a hint as to how the buffer will be used
		// vertexDecl	describes the layout of vertex attributes within the buffer.
		VertexBuffer(size_t size, BufferUsage::Enum usage, boost::shared_ptr<VertexDeclaration> vertexDecl);

		const size_t Size;
		const BufferUsage::Enum Usage;
		const boost::shared_ptr<VertexDeclaration> VertexDecl;

		// Make the current buffer "active".
		void Activate();

		// Make NO vertex buffer active.
		void Deactivate();

		// Copy size bytes of data into the buffer, starting at offset.
		void Set(const void* const data, size_t size, size_t offset);

		// Copy size bytes, starting at offset from the buffer into the data area.
		void Get(size_t size, size_t offset, void* const data);

	protected:
		~VertexBuffer();

	private:
		struct Impl;
		boost::scoped_ptr<Impl> impl;
	};
}

#endif // __CORE_VERTEXBUFFER__
