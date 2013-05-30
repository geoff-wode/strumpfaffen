#if ! defined(__MODEL_H__)
#define __MODEL_H__

#include <buffers.h>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

class Model
{
public:
	static boost::shared_ptr<Model> New();

	virtual ~Model();

	bool Load(const std::string& filename);

	void Render() const;
private:
	Model();

	boost::shared_ptr<StaticBuffer> dataBuffer;

	GLuint vao;
	unsigned int vertexCount;
};

#endif // __MODEL_H__
