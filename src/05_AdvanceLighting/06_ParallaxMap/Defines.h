#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <LOGL/CommonDefine.h>
#include <Utility/Config.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Define {
	const std::string str_Chapter = "05_AdvanceLighting";
	const std::string str_Subchapter = "06_ParallaxMap";
	const std::string str_WindowTitle = str_Chapter + "/" + str_Subchapter;


	//------------ shader
	const std::string str_ShaderPrefix = "/data/shaders/" + str_Chapter + "/" + str_Subchapter + "/";

	const std::string str_ParallaxMap = "parallaxMap";
	const std::string str_ParallaxMap_vs = str_ShaderPrefix + str_ParallaxMap + str_VsPostfix;
	const std::string str_ParallaxMap_fs = str_ShaderPrefix + str_ParallaxMap + str_FsPostfix;


	Ubpa::Config * DoConfig();
}

#endif // !_DEFINES_H_


