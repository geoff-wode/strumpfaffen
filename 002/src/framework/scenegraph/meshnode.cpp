#include <framework/scenegraph/scene.h>
#include <framework/scenegraph/meshnode.h>
#include <boost/make_shared.hpp>

using namespace Framework;

//------------------------------------------------------------------

struct VertexPosition
{
	glm::vec3 position;
};

static VertexPosition vertices[] =
{
	glm::vec3(-1, -1, 0),
	glm::vec3( 1, -1, 0),
	glm::vec3( 0,  1, 0)
};

//-----------------------------------------------------------------

MeshNode::MeshNode()
{
}

MeshNode::~MeshNode()
{
}

void MeshNode::LoadResources(Scene* const scene)
{
	drawState.vertexLayout = boost::make_shared<VertexLayout>();
	
	drawState.vertexLayout->stride = sizeof(VertexPosition);
	drawState.vertexLayout->vertices.push_back(VertexElement(GL_FLOAT, offsetof(VertexPosition, position), 3));

	drawState.vertexBuffer = boost::make_shared<VertexBuffer>(sizeof(vertices), Framework::BufferUsageHint::Static);
	drawState.vertexBuffer->SetData(vertices, sizeof(vertices), 0);

	drawState.shader = scene->device->CreateShader("shaders/simple");
}

void MeshNode::Render(Scene* const scene)
{
	scene->device->Render(drawState, 1, Framework::PrimitiveType::Triangles);
}
