#if ! defined(__PROGRAM__)
#define __PROGRAM__

#include <device.h>
#include <buffer.h>
#include <shaders/shaderprogram.h>
#include <boost/shared_ptr.hpp>
#include <geometry/vertexarray.h>

class Program
{
public:
	Program(boost::shared_ptr<Device> device);
	virtual ~Program();
	void Run();
	void Quit();

protected:
	virtual bool Initialise();
	virtual void LoadResources();
	virtual void Update(unsigned int elapsedMS);
	virtual void Render();
	virtual void Quitting();

private:
	bool quit;
	boost::shared_ptr<Device> device;

	boost::shared_ptr<VertexArray> vertexArray;
	
	struct TurningShader
	{
		float angle;
		boost::shared_ptr<ShaderProgram> shader;
		boost::shared_ptr<ShaderUniform> angleParam;
	} turningShader;

	ClearState clearState;
	RenderState renderState;

	void HandleEvents();
};

#endif // __PROGRAM__
