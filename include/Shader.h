#ifndef SHADER_H_
#define SHADER_H_

#include <string>
#include <GLFW/Glfw.h>
#include <Ubpa/Utility.h>

namespace LOGL {
	class Shader{
	public:
		// constructor generates the shader on the fly
		Shader(const char* vertexPath, const char* fragmentPath);

		size_t GetID() const;
		bool IsValid() const;

		// activate the shader
		void Use();

		// utility uniform functions
		void SetBool(const std::string &name, bool value) const;
		void SetInt(const std::string &name, int value) const;
		void SetFloat(const std::string &name, float value) const;
		void SetVec4f(const std::string &name, float value0, float value1, float value2, float value3) const;
	private:
		// utility function for checking shader compilation/linking errors.
		int CheckCompileErrors(size_t shader, std::string type);
		//------------
		size_t ID;
		bool valid;
	};
}

#endif // !SHADER_H_
