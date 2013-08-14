#if ! defined(__IBUFFER_H__)
#define __IBUFFER_H__

#include <stddef.h>
#include <gl_loader/gl_loader.h>
#include <boost/shared_ptr.hpp>

namespace Framework
{
	namespace BufferUsageHint
	{
		enum Enum
		{
			Static = GL_STATIC_DRAW,
			Dynamic = GL_DYNAMIC_DRAW,
			Stream = GL_STREAM_DRAW
		};
	}

	class Buffer
	{
	public:
		virtual ~Buffer()
		{
			glDeleteBuffers(1, &handle);
		}

		void GetData(void* const data, size_t size, size_t offset)
		{
			glBindBuffer(type, handle);
			glGetBufferSubData(type, offset, size, data);
			glBindBuffer(type, 0);
		}

		void SetData(const void* const data, size_t size, size_t offset)
		{
			glBindBuffer(type, handle);
			glBufferSubData(type, offset, size, data);
			glBindBuffer(type, 0);
		}

		void Enable() { glBindBuffer(type, handle); }
		void Disable() { glBindBuffer(type, 0); }

		const size_t Size;

	protected:
		Buffer(GLenum type, size_t size, GLenum usage)
			: type(type), Size(size)
		{
			glGenBuffers(1, &handle);
			glBindBuffer(type, handle);
			glBufferData(type, size, NULL, usage);
			glBindBuffer(type, 0);
		}

		GLenum type;
		GLuint handle;
	};

	class VertexBuffer : public Buffer
	{
	public:
		VertexBuffer(size_t size, BufferUsageHint::Enum usage)
			: Buffer(GL_ARRAY_BUFFER, size, usage)
		{
		}
	};
	typedef boost::shared_ptr<VertexBuffer> VertexBufferPtr;

	namespace IndexBufferDataType
	{
		enum Enum
		{
			Short = GL_UNSIGNED_SHORT,
			Int = GL_UNSIGNED_INT
		};
	}

	class IndexBuffer : public Buffer
	{
	public:
		IndexBuffer(size_t size, IndexBufferDataType::Enum dataType, BufferUsageHint::Enum usage)
			: Buffer(GL_ELEMENT_ARRAY_BUFFER, size, usage), DataType(dataType)
		{
		}

		const IndexBufferDataType::Enum DataType;
	};
	typedef boost::shared_ptr<IndexBuffer> IndexBufferPtr;

	class ShaderUniformBuffer : public Buffer
	{
	public:
		ShaderUniformBuffer(size_t size)
			: Buffer(GL_UNIFORM_BUFFER, size, GL_DYNAMIC_DRAW)
		{
		}
	};
	typedef boost::shared_ptr<ShaderUniformBuffer> ShaderUniformBufferPtr;
}

#endif // __IBUFFER_H__
