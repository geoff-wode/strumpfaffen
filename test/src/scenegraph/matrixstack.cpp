#include <scenegraph/matrixstack.h>

MatrixStack::MatrixStack()
{
	stack.push(glm::mat4(1));
}

void MatrixStack::Push(const glm::mat4& value)
{
	stack.push(value * stack.top());
}

glm::mat4 MatrixStack::Peek() const
{
	return stack.top();
}

void MatrixStack::Pop()
{
	if (stack.size() > 1)
	{
		stack.pop();
	}
}
