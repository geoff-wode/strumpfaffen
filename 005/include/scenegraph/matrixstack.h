#if ! defined(__MATRIX_STACK__)
#define __MATRIX_STACK__

#include <stack>
#include <glm/glm.hpp>

// Provides a simple mechanism for matrix concatenation.
// The stack is initialised with an identity matrix as its
// first entry (and this cannot be popped).
class MatrixStack
{
public:
	MatrixStack()
	{
		stack.push(glm::mat4(1));
	}

	// Concatenate the value with the current top-of-stack matrix and push
	// the result onto the stack.
	void Push(const glm::mat4& value)
	{
		stack.push(value * stack.top());
	}

	// Remove the matrix at the stop of the stack.
	void Pop()
	{
		if (stack.size() > 1)
		{
			stack.pop();
		}
	}

	// Return the value currently at the top of the stack.
	glm::mat4 Top()
	{
		return stack.top();
	}

private:
	std::stack<glm::mat4> stack;
};

#endif // __MATRIX_STACK__
