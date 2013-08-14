#if ! defined(__DRAW_STATE_H__)
#define __DRAW_STATE_H__

#include <vector>
#include <glm/glm.hpp>
#include <gl_loader/gl_loader.h>
#include <boost/shared_ptr.hpp>
#include <framework/shaders/shader.h>
#include <framework/buffers/buffers.h>

namespace Framework
{
	// Defines a single vertex element (position, normal, etc.) within an interleaved
	// vertex stream.
	struct VertexElement
	{
		VertexElement()
		{
		}

		VertexElement(GLenum type, GLuint offset, GLint components)
			: type(type), offset(offset), components(components)
		{
		}

		GLenum type;				// type of component making up the element (e.g. glm::vec3 == GL_FLOAT)
		GLuint offset;			// byte offset from start of vertex
		GLint components;		// count of how many components there are (e.g. glm::vec3 == 3)
	};

	struct VertexLayout
	{
		size_t stride;			// byte distance from one vertex to the next in the stream
		std::vector<VertexElement> vertices;
	};
	typedef boost::shared_ptr<VertexLayout> VertexLayoutPtr;

	namespace PolygonMode
	{
		enum Enum
		{
			Point,
			Line,
			Fill
		};
	};

	struct RenderState
	{
		RenderState()
			: depthMask(true),
				colourMask(true),
				polygonMode(PolygonMode::Fill)
		{
		}

		void Set(const RenderState& newState)
		{
			if (newState.depthMask != depthMask)
			{
				depthMask = newState.depthMask;
				glDepthMask(depthMask);
			}
			if (newState.colourMask != colourMask)
			{
				colourMask = newState.colourMask;
				glColorMask(colourMask.r, colourMask.g, colourMask.b, colourMask.a);
			}
			if (newState.polygonMode != polygonMode)
			{
				polygonMode = newState.polygonMode;
				switch (polygonMode)
				{
				case PolygonMode::Fill: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
				case PolygonMode::Line: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
				case PolygonMode::Point: glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); break;
				}
			}
		}

		void ForceDefaults()
		{
			glDepthMask(depthMask);
			glColorMask(colourMask.r, colourMask.g, colourMask.b, colourMask.a);
			switch (polygonMode)
			{
			case PolygonMode::Fill: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); break;
			case PolygonMode::Line: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); break;
			case PolygonMode::Point: glPolygonMode(GL_FRONT_AND_BACK, GL_POINT); break;
			}
		}

		bool depthMask;					// enable or disable writing into the depth buffer (default: true)
		glm::bvec4 colourMask;	// enable and disable writing of frame buffer color components (default: all true)
		PolygonMode::Enum polygonMode;	// default: fill
	};

	namespace PrimitiveType
	{
		enum Enum
		{
			Triangles,
			TriangleStrip
		};
	}

	struct DrawState
	{
		RenderState renderState;
		ShaderPtr shader;
		VertexLayoutPtr vertexLayout;
		VertexBufferPtr vertexBuffer;
		IndexBufferPtr indexBuffer;

		void Set(const DrawState& newState)
		{
			renderState.Set(newState.renderState);

			if (newState.shader != shader)
			{
				shader = newState.shader;
				shader->Enable();
				shader->Apply();
			}

			if (newState.indexBuffer != indexBuffer)
			{
				// If the new state has an index buffer defined, switch to it...
				if (newState.indexBuffer)
				{
					newState.indexBuffer->Enable();
				}
				else
				{
					// otherwise, no index buffer is defined...
					indexBuffer->Disable();
				}
				indexBuffer = newState.indexBuffer;
			}

			if (newState.vertexBuffer != vertexBuffer)
			{
				vertexBuffer = newState.vertexBuffer;
				vertexBuffer->Enable();
			}

			if (newState.vertexLayout != vertexLayout)
			{
				// Disable the old vertex attributes before enabling the new ones...
				if (vertexLayout)
				{
					for (size_t i = 0; i < vertexLayout->vertices.size(); ++i)
					{
						glDisableVertexAttribArray(i);
					}
				}
				vertexLayout = newState.vertexLayout;
				for (size_t i = 0; i < vertexLayout->vertices.size(); ++i)
				{
					glEnableVertexAttribArray(i);
					glVertexAttribPointer(i,
						vertexLayout->vertices[i].components,
						vertexLayout->vertices[i].type,
						GL_FALSE,
						vertexLayout->stride,
						(const void*)vertexLayout->vertices[i].offset);
				}
			}
		}
	};
}

#endif // __DRAW_STATE_H__
