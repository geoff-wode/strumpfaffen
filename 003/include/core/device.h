// Performs primitive-based rendering, creates resources, handles system-level variables
// and creates shaders. 

#if ! defined(__CORE_DEVICE__)
#define __CORE_DEVICE__

#include <map>
#include <string>
#include <glm/glm.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

#include <core/clearstate.h>
#include <core/renderstate.h>
#include <core/vertexbuffer.h>
#include <core/shaders/shader.h>

namespace core
{
	namespace PrimitiveType
	{
		enum Enum
		{
			Triangles,			// A basic "soup" of triangles defined by groups of 3 vertices.
			TriangleStrip		// Each triangle is defined by the previous 2 vertices and a new vertex.
		};
	}

	class Device
	{
	public:
		Device();

		// Clear buffers.
		void Clear(ClearBufferType::Enum buffers, const ClearState& state);

		// Render primitives of the given type using the given render state.
		void DrawPrimitives(
			PrimitiveType::Enum type,
			size_t numPrimitives,
			size_t startVertex,
			const RenderState& state);

		boost::shared_ptr<Shader> CreateShader(const std::string& name);

		// These values are shared by all shaders and are set automatically.
		struct ShaderGlobals
		{
			glm::mat4 WorldMatrix;
			glm::mat4 ViewMatrix;
			glm::mat4 ProjectionMatrix;
			glm::vec3 CameraPosition;
		} shaderGlobals;

	protected:
		~Device();

	private:
		struct Impl;
		boost::scoped_ptr<Impl> impl;
	};
}

#endif // __CORE_DEVICE__
