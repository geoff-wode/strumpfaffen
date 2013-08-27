#if ! defined(__MATRIX_STACK__)
#define __MATRIX_STACK__

#include <stack>
#include <glm/glm.hpp>
#include <boost/noncopyable.hpp>

// Maintains a hierarchy of concatenated matrices.
// The current top-of-stack is always either the identity matrix or the result
// of concatenating the current node's transform matrix with its parent's matrices.
class MatrixStack : public boost::noncopyable
{
public:
	MatrixStack();

	// Insert a new matrix onto the stack which is the concatenation of the current
	// top-of-stack and the input value. I.e., new_top = value * old_top
	void Push(const glm::mat4& value);

	// Return a copy of the current value at the top of the stack.
	glm::mat4 Peek() const;

	// Remove the current value at the top of the stack.
	void Pop();

private:
	std::stack<glm::mat4> stack;
};

#endif // __MATRIX_STACK__
