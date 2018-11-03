#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <LOGL/CommonDefine.h>
#include <Utility/Config.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Define {
	const std::string str_Chapter = "05_AdvanceLighting";
	const std::string str_Subchapter = "01_BlinnPhong";

	//------------ shader
	const std::string str_ShaderPrefix = "/data/shaders/" + str_Chapter + "/" + str_Subchapter + "/";

	const std::string str_BlinnPhong = "BlinnPhong";
	const std::string str_BlinnPhong_vs = str_ShaderPrefix + str_BlinnPhong + ".vs";
	const std::string str_BlinnPhong_fs = str_ShaderPrefix + str_BlinnPhong + ".fs";

	//------------ data

	Ubpa::Config * DoConfig();
}

#endif // !_DEFINES_H_


