#if ! defined(__DEVICE_H__)
#define __DEVICE_H__

#include <map>
#include <string>
#include <gl_loader/gl_loader.h>
#include <glm/glm.hpp>
#include <framework/clearstate.h>
#include <framework/drawstate.h>
#include <framework/shaders/shader.h>

namespace Framework
{
	class Device
	{
	public:
		Device(const glm::ivec2& backbufferSize, bool fullScreen = false);
		~Device();

		bool Initialise();

		void Clear(const ClearState& clearState);
		void Render(const DrawState& drawState, size_t primitiveCount, PrimitiveType::Enum type);

		ShaderPtr CreateShader(const std::string& name);

		std::map<std::string, GLuint> uniformBufferBindings;

	private:
		bool fullScreen;
		glm::ivec2 backbufferSize;

		ClearState clearState;
		DrawState drawState;
	};
}

#endif // __DEVICE_H__
