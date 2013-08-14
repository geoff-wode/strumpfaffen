#if ! defined(__SCENEGRAPH_MATRIXSTACK_H__)
#define __SCENEGRAPH_MATRIXSTACK_H__

#include <stack>
#include <glm/glm.hpp>

namespace Framework
{
	class MatrixStack
	{
	public:
		MatrixStack() { }
		~MatrixStack() { }

		void Push(const glm::mat4& m)
		{
			if (!stack.empty())
			{
				const glm::mat4 transform = stack.top() * m;
				stack.push(transform);
			}
			else
			{
				stack.push(m);
			}
		}

		void Pop()
		{
			if (!stack.empty())
			{
				stack.pop();
			}
		}

		glm::mat4 Top() const
		{
			if (!stack.empty())
			{
				return stack.top();
			}
			else
			{
				return glm::mat4(1);
			}
		}

	private:
		std::stack<glm::mat4> stack;
	};
}

#endif // __SCENEGRAPH_MATRIXSTACK_H__
