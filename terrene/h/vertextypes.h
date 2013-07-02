#if ! defined(__VERTEX_TYPES_H__)
#define __VERTEX_TYPES_H__

#include <string>
#include <gl_loader/gl_loader.h>
#include <glm/glm.hpp>

namespace Terrene
{
#pragma pack(push,1)

	// Vertex arrays are always interleaved.
	struct VertexArrayElement
	{
		VertexArrayElement(const std::string& name, GLenum type, size_t components, size_t size, size_t offset);

		const std::string Name;
		const GLenum Type;
		const size_t Components;
		const size_t Size;
		const size_t Offset;
	};

	struct VertexDeclaration
	{
		VertexDeclaration(unsigned int numElements, const VertexArrayElement* const elements);

		const unsigned int Stride;
		const unsigned int NumElements;
		const VertexArrayElement* const Elements;
	};

	struct VertexPositionTexture
	{
		glm::vec3 Position;
		glm::vec2 TexCoord;

		static const VertexDeclaration VertexDecl;
	};

	struct VertexPositionNormalTexture
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoord;

		static const VertexDeclaration VertexDecl;
	};

#pragma pack(pop)

}

#endif // __VERTEX_TYPES_H__
