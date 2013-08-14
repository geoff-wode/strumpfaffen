#if ! defined(__DEVICE__)
#define __DEVICE__

#include <glm/glm.hpp>
#include <viewport.h>
#include <clearstate.h>
#include <renderstate.h>

class Device
{
public:
	Device(const glm::ivec2& backbufferSize);

	virtual ~Device();

	bool Initialise();

	void SetViewport(const Viewport& viewport);
	Viewport GetViewport() const;

	void Clear(const ClearState& state);
	void Draw(GLenum primitiveType, size_t primitiveCount, const RenderState& state);

private:
	Viewport viewport;
	ClearState clearState;
	RenderState renderState;
	glm::ivec2 backbufferSize;

	void ApplyClearState(const ClearState& newState);
	void ApplyVertexDecl(const VertexDeclaration* const decl);
};

#endif // __DEVICE__
