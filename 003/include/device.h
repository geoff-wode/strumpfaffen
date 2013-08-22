#if ! defined(__DEVICE__)
#define __DEVICE__

#include <map>
#include <SDL.h>
#include <glm/glm.hpp>
#include <viewport.h>
#include <clearstate.h>
#include <renderstate.h>
#include <boost/make_shared.hpp>

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
	void DrawIndexed(GLenum primitiveType, size_t indexCount, GLenum indexType, const RenderState& state);

	void SwapBuffers() const;

	static void CreateBlockBuffer(const std::string& name, size_t size);
	static GLuint GetBlockBindingIndex(const std::string& name);
	static boost::shared_ptr<Buffer> GetBlockBuffer(const std::string& name);

	glm::mat4 WorldMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;

private:
	SDL_Window* mainWindow;
	Viewport viewport;
	ClearState clearState;
	RenderState renderState;
	glm::ivec2 backbufferSize;

	typedef std::map<std::string,boost::shared_ptr<Buffer>> UniformBlockBufferMap;
	static UniformBlockBufferMap blocks;

	boost::shared_ptr<Buffer> activeVertexBuffer;
	boost::shared_ptr<Buffer> activeIndexBuffer;
	boost::shared_ptr<ShaderProgram> activeShader;

	void ApplyClearState(const ClearState& newState);
	void ApplyRenderState(const RenderState& newState);
	void ApplyColourMask(const glm::bvec4& mask);
	void ApplyDepthMask(bool mask);
	void ApplyShaderProgram(boost::shared_ptr<ShaderProgram> shader);
	void ApplyVertexArray(const VertexDeclaration* const decl);
	void ApplyVertexBuffer(boost::shared_ptr<Buffer> buffer);
	void ApplyIndexBuffer(boost::shared_ptr<Buffer> buffer);
};

#endif // __DEVICE__
