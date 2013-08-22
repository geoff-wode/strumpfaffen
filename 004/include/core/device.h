// Performs primitive-based rendering, creates resources, handles system-level variables
// and creates shaders. 

#if ! defined(__CORE_DEVICE__)
#define __CORE_DEVICE__

#include <SDL.h>
#include <gl_loader/gl_loader.h>
#include <map>
#include <string>
#include <glm/glm.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

#include <core/clearstate.h>
#include <core/renderstate.h>
#include <core/shaders/shader.h>
#include <core/viewport.h>
#include <core/uniformbuffer.h>

namespace core
{
	class Device
	{
	public:
		Device();
		~Device();

		bool Initialise();

		// Clear buffers.
		void Clear(ClearBufferType::Enum buffers, const ClearState& state);

		// Render primitives of the given type using the given render state.
		void DrawPrimitives(
			GLenum type,
			size_t numPrimitives,
			size_t startVertex,
			const RenderState& state);

		void SetViewport(const Viewport& viewport);
		Viewport GetViewport() const;

		void SetCameraPosition(const glm::vec3& value) { cameraPosition = value; }
		void SetWorldMatrix(const glm::mat4& value) { worldMatrix = value; }
		void SetViewMatrix(const glm::mat4& value) { viewMatrix = value; }
		void SetProjectionMatrix(const glm::mat4& value) { projectionMatrix = value; }
		glm::vec3 GetCameraPosition() const { return cameraPosition; }
		glm::mat4 GetetWorldMatrix() const { return worldMatrix; }
		glm::mat4 GetetViewMatrix() const { return viewMatrix; }
		glm::mat4 GetetProjectionMatrix() const { return projectionMatrix; }

		void SwapBuffers();

		size_t BackbufferWidth;
		size_t BackbufferHeight;

	protected:
		SDL_Window* mainWindow;
		SDL_GLContext glContext;

		Viewport viewport;
		RenderState renderState;
		ClearState clearState;

		glm::mat4 worldMatrix;
		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;
		glm::vec3 cameraPosition;

		boost::shared_ptr<UniformBuffer> shaderGlobalsBuffer;
	};
}

#endif // __CORE_DEVICE__
