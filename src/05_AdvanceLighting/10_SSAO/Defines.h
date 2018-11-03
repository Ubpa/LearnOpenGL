#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <LOGL/CommonDefine.h>
#include <Utility/Config.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Define {
	const std::string str_Chapter = "05_AdvanceLighting";
	const std::string str_Subchapter = "10_SSAO";
	const std::string str_WindowTitle = str_Chapter + "/" + str_Subchapter;


	//------------ shader
	const std::string str_ShaderPrefix = "/data/shaders/" + str_Chapter + "/" + str_Subchapter + "/";

	const std::string str_GBuffer = "gBuffer";
	const std::string str_GBuffer_vs = str_ShaderPrefix + str_GBuffer + str_VsPostfix;
	const std::string str_GBuffer_fs = str_ShaderPrefix + str_GBuffer + str_FsPostfix;
	
	const std::string str_SSAO = "SSAO";
	const std::string str_SSAO_vs = str_Basic_P2T2_vs;
	const std::string str_SSAO_fs = str_ShaderPrefix + str_SSAO + str_FsPostfix;

	const std::string str_Blur = "blur";
	const std::string str_Blur_vs = str_Basic_P2T2_vs;
	const std::string str_Blur_fs = str_ShaderPrefix + str_Blur + str_FsPostfix;

	const std::string str_DeferedShading = "deferedShading";
	const std::string str_DeferedShading_vs = str_Basic_P2T2_vs;
	const std::string str_DeferedShading_fs = str_ShaderPrefix + str_DeferedShading + str_FsPostfix;


	Ubpa::Config * DoConfig();
}

#endif // !_DEFINES_H_


