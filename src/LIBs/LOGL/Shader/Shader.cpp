#include <LOGL/Shader.h>
#include <string>

using namespace std;
using namespace LOGL;
using namespace Ubpa;

Shader::Shader(const std::string & vertexPath, const std::string & fragmentPath) {
	valid = true;
	File vsF(vertexPath, File::Mode::READ);
	File fsF(fragmentPath, File::Mode::READ);
	string vsStr = vsF.ReadAll();
	string fsStr = fsF.ReadAll();
	if (vsStr.size() == 0 || fsStr.size() == 0) {
		cout << vertexPath << " or " << fragmentPath << " read failed.\n";
		valid = false;
		return;
	}
	const char * vsCStr = vsStr.c_str();
	const char * fsCStr = fsStr.c_str();
	//------------
	// vertex shader
	size_t glvs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(glvs, 1, &vsCStr, NULL);
	glCompileShader(glvs);
	if (!CheckCompileErrors(glvs, "VERTEX")) {
		cout << vertexPath << " compiles error\n";
		valid = false;
		return;
	}
	// fragment Shader
	size_t glfs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(glfs, 1, &fsCStr, NULL);
	glCompileShader(glfs);
	if(!CheckCompileErrors(glfs, "FRAGMENT")) {
		cout << fragmentPath << " compiles error\n";
		valid = false;
		return;
	}
	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, glvs);
	glAttachShader(ID, glfs);
	glLinkProgram(ID);
	if (!CheckCompileErrors(ID, "PROGRAM")) {
		cout << vertexPath << " and " << fragmentPath << " link failed.\n";
		valid = false;
		return;
	}
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(glvs);
	glDeleteShader(glfs);
}

size_t Shader::GetID() const { return ID; }
bool Shader::IsValid() const { return valid; }

bool Shader::Use() {
	if (valid)
		glUseProgram(ID);
	return valid;
}

void Shader::SetBool(const std::string &name, bool value) const{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string &name, int value) const{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const string &name, float value) const{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetVec3f(const std::string &name, float value0, float value1, float value2) const {
	glUniform3f(glGetUniformLocation(ID, name.c_str()), value0, value1, value2);
}

void Shader::SetVec3f(const std::string &name, const glm::vec3 & v) const {
	SetVec3f(name, v.x, v.y, v.z);
}

void Shader::SetVec4f(const string &name, float value0, float value1, float value2, float value3) const{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), value0, value1, value2, value3);
}

void Shader::SetVec4f(const std::string &name, const glm::vec4 & v) const {
	SetVec4f(name, v.x, v.y, v.z, v.w);
}

void Shader::SetMat4f(const std::string &name, const float * matValue) {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, matValue);
}

void Shader::SetMat4f(const std::string &name, const glm::mat4 mat4) {
	SetMat4f(name, glm::value_ptr(mat4));
}

int Shader::CheckCompileErrors(size_t shader, string type){
	int success;
	char infoLog[1024];
	if (type != "PROGRAM"){
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success){
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}else{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	return success;
}