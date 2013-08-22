#include <scenegraph/matrixstack.h>


MatrixStack::MatrixStack()
	: stack(new std::stack<glm::mat4>())
{
	stack->push(glm::mat4(1));
}

MatrixStack::~MatrixStack()
{
}

void MatrixStack::Push(const glm::mat4& matrix)
{
	const glm::mat4 transform(matrix * stack->top());
	stack->push(transform);
}

glm::mat4 MatrixStack::Peek() const
{
	return stack->top();
}

glm::mat4 MatrixStack::Pop()
{
	const glm::mat4 transform(stack->top());
	if (stack->size() > 1)
	{
		stack->pop();
	}
	return transform;
}
