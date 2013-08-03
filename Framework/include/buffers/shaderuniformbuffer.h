// Declares a buffer used to store shader parameter blocks.

#if ! defined(__SHADER_UNIFORM_BUFFER_H__)
#define __SHADER_UNIFORM_BUFFER_H__

#include <boost/shared_ptr.hpp>
#include <buffers/bufferusagehint.h>

namespace Buffers
{

	class IShaderUniformBuffer
	{
	public:
		virtual void SetData(const void* const data, size_t count, size_t startIndex) const = 0;

		virtual void GetData(void* const data, size_t count, size_t startIndex) const = 0;

		virtual void Activate() const = 0;
		virtual void Deactivate() const = 0;

		virtual size_t SizeInBytes() const = 0;
	};

	typedef boost::shared_ptr<IShaderUniformBuffer> ShaderUniformBuffer;

	ShaderUniformBuffer CreateShaderUniformBuffer(unsigned int bindingPoint, size_t sizeInBytes);

}

#endif // __SHADER_UNIFORM_BUFFER_H__
