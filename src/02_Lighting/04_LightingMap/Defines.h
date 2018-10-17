#ifndef _DEFINE_H_
#define _DEFINE_H_

#include <LOGL/CommonDefine.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Define {
	const std::string str_Chapter = "02_Lighting";
	const std::string str_Subchapter = "04_LightingMap";

	const std::string str_ShaderPrefix = "/data/shaders/" + str_Chapter + "/" + str_Subchapter + "/";
	const std::string str_Light_vs = str_ShaderPrefix + "light.vs";
	const std::string str_Light_fs = str_ShaderPrefix + "light.fs";
	const std::string str_Lighting_vs = str_ShaderPrefix + "lighting.vs";
	const std::string str_Lighting_fs = str_ShaderPrefix + "lighting.fs";

	const std::string str_LightColor = "lightColor";
	const std::string str_LightPos = "lightPos";
}

#endif // !_DEFINE_H_


