#if ! defined(__VERTEX_DECLARATION_H__)
#define __VERTEX_DECLARATION_H__

#include <stddef.h>

struct VertexComponent
{
	GLenum	type;
	size_t	componentCount;
	size_t	offset;
};

struct VertexDeclaration
{
	const size_t stride;
	const size_t componentCount;
	const VertexComponent* const components;
};

#endif // __VERTEX_DECLARATION_H__
