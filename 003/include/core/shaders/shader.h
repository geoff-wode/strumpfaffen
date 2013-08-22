#if ! defined(__CORE_SHADER__)
#define __CORE_SHADER__

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <core/attributetype.h>
#include <core/shaders/uniform.h>
#include <boost/scoped_ptr.hpp>

namespace core
{
	class Shader
	{
	public:
		~Shader();

		// Make the current shader active.
		void Activate();
		static void Deactivate();

		// Ensure that all uniform variables are up-to-date.
		void Update();

		ShaderUniform* const GetUniform(const std::string& name) const;

	private:
		Shader(const std::string& name);
		struct Impl;
		boost::scoped_ptr<Impl> impl;

		friend class Device;
	};
}

#endif // __CORE_SHADER__
