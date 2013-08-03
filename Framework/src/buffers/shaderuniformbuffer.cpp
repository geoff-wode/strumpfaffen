#include <gl_loader/gl_loader.h>
#include <buffers/shaderuniformbuffer.h>
#include <util/debug.h>

using namespace Buffers;

//-----------------------------------------------------------------------

class ShaderUniformBufferImpl : public IShaderUniformBuffer
{
public:
	ShaderUniformBufferImpl(unsigned int bindingPoint, size_t sizeInBytes)
		: sizeInBytes(sizeInBytes), usageHint(usageHint)
	{
		glGenBuffers(1, &handle);

		Activate();

		// IShader block buffers are always dynamic...
		glBufferData(GL_UNIFORM_BUFFER, sizeInBytes, NULL, GL_DYNAMIC_DRAW);

		// Link the buffer to the shader block...
		glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, handle);
		
		Deactivate();
	}

	virtual ~ShaderUniformBufferImpl()
	{
		glDeleteBuffers(1, &handle);
	}

	virtual void SetData(const void* const data, size_t dataLength, size_t offset) const
	{
		ASSERT(dataLength + offset <= sizeInBytes);

		glBufferSubData(GL_UNIFORM_BUFFER, offset, dataLength, data);
	}

	virtual void GetData(void* const data, size_t dataLength, size_t offset) const
	{
		ASSERT(dataLength + offset <= sizeInBytes);

		glGetBufferSubData(GL_UNIFORM_BUFFER, offset, dataLength, data);
	}

	virtual void Activate() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, handle);
	}

	virtual void Deactivate() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	virtual size_t SizeInBytes() const
	{
		return sizeInBytes;
	}

private:
	size_t sizeInBytes;
	UsageHint::Enum usageHint;
	GLuint handle;
};

//-----------------------------------------------------------------------

ShaderUniformBuffer Buffers::CreateShaderUniformBuffer(unsigned int bindingPoint, size_t sizeInBytes)
{
	ShaderUniformBuffer buffer(new ShaderUniformBufferImpl(bindingPoint, sizeInBytes));
	return buffer;
}
