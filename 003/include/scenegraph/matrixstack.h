#if ! defined(__MATRIX_STACK__)
#define __MATRIX_STACK__

#include <stack>
#include <glm/glm.hpp>
#include <boost/scoped_ptr.hpp>

// Provides a means of performing hierarchical matrix concatenation.
// The matrix at the top of the stack is always either the identity
// matrix or the result of multiplying all previous matrices in the
// stack.
class MatrixStack
{
public:
	MatrixStack();
	~MatrixStack();

	// Multiply the current top-of-stack matrix by the given matrix, storing
	// the result as the new top-of-stack.
	void Push(const glm::mat4& matrix);

	// Return the matrix currently at the top of the stack.
	glm::mat4 Peek() const;

	// Pop the top-most matrix from the stack and return it.
	glm::mat4 Pop();

private:
	boost::scoped_ptr<std::stack<glm::mat4>> stack;
};

#endif // __MATRIX_STACK__
