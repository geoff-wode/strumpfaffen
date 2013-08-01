#include <vector>
#include <string>
#include <stdio.h>
#include <boost/shared_ptr.hpp>
#include <glm/glm.hpp>



struct Attr
{
	struct Types
	{
		enum Enum
		{
			Vec2,
			Vec3
		};
	};
	
	std::string Name;
	Types::Enum Type;
	size_t			Size;
};

class IV
{
public:
	virtual int AttrCount() const = 0;
	virtual const Attr* const Attrs() const = 0;
};

class VP : public IV
{
public:
	VP() { }
	VP(const glm::vec3& p) : P(p) { }
	VP(float x, float y, float z) : P(x,y,z) { }

	glm::vec3 P;

	virtual int AttrCount() const { return attrCount; }
	virtual const Attr* const Attrs() const { return attribs; }

private:
	static const size_t attrCount;
	static const Attr attribs[];
};

const Attr VP::attribs[] =
{
	{ "Position", Attr::Types::Vec3, sizeof(P) }
};
const size_t VP::attrCount = sizeof(VP::attribs) / sizeof(VP::attribs[0]);

//--------------------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
	std::vector<VP> p;

	p.push_back(VP(1,1,1));
	p.push_back(VP(2,2,2));

	for (int i = 0; i < p[0].AttrCount(); ++i)
	{
		printf("%s: %d\n", p[0].Attrs()[i].Name.c_str(), p[0].Attrs()[i].Size);
		for (size_t v = 0; v < p.size(); ++v)
		{
			printf("  (%f, %f, %f)\n", p[v].P.x, p[v].P.y, p[v].P.z);
		}
	}

	return 0;
}
