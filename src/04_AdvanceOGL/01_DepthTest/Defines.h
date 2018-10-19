#ifndef _DEFINE_H_
#define _DEFINE_H_

#include <LOGL/CommonDefine.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Define {
	const std::string str_Chapter = "04_AdvanceOGL";
	const std::string str_Subchapter = "01_DepthTest";

	const std::string str_ShaderPrefix = "/data/shaders/" + str_Chapter + "/" + str_Subchapter + "/";
	const std::string str_Light_vs = str_ShaderPrefix + "light.vs";
	const std::string str_Light_fs = str_ShaderPrefix + "light.fs";
	const std::string str_Lighting_vs = str_ShaderPrefix + "lighting.vs";
	const std::string str_Lighting_fs = str_ShaderPrefix + "lighting.fs";
	const std::string str_Nanosuit_vs = str_ShaderPrefix + "nanosuit.vs";
	const std::string str_Nanosuit_fs = str_ShaderPrefix + "nanosuit.fs";

	const std::string str_LightColor = "lightColor";
	const std::string str_LightPos = "lightPos";

	// positions of the point lights
	const glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};
}

#endif // !_DEFINE_H_


