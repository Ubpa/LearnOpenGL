#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <LOGL/CommonDefine.h>
#include <Utility/Config.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Define {
	const std::string str_Chapter = "05_AdvanceLighting";
	const std::string str_Subchapter = "05_NormalMap";
	const std::string str_WindowTitle = str_Chapter + "/" + str_Subchapter;


	//------------ shader
	const std::string str_ShaderPrefix = "/data/shaders/" + str_Chapter + "/" + str_Subchapter + "/";

	const std::string str_NormalMap = "normalMap";
	const std::string str_NormalMap_vs = str_ShaderPrefix + str_NormalMap + str_VsPostfix;
	const std::string str_NormalMap_fs = str_ShaderPrefix + str_NormalMap + str_FsPostfix;


	Ubpa::Config * DoConfig();
}

#endif // !_DEFINES_H_


