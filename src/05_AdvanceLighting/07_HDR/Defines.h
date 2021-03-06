#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <LOGL/CommonDefine.h>
#include <Utility/Config.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Define {
	const std::string str_Chapter = "05_AdvanceLighting";
	const std::string str_Subchapter = "07_HDR";
	const std::string str_WindowTitle = str_Chapter + "/" + str_Subchapter;


	//------------ shader

	const std::string str_ShaderPrefix = "/data/shaders/" + str_Chapter + "/" + str_Subchapter + "/";

	const std::string str_HDR = "hdr";
	const std::string str_HDR_vs = str_Basic_P2T2_vs;
	const std::string str_HDR_fs = str_ShaderPrefix + str_HDR + str_FsPostfix;

	const std::string str_Lighting = "lighting";
	const std::string str_Lighting_vs = str_ShaderPrefix + str_Lighting + str_VsPostfix;
	const std::string str_Lighting_fs = str_ShaderPrefix + str_Lighting + str_FsPostfix;


	//------------ data

	const glm::vec3 data_LightPositions[] = {
		glm::vec3(0.0f, 0.0f, 49.5f),
		glm::vec3(-1.4f, -1.9f, 9.0f),
		glm::vec3(0.0f, -1.8f, 4.0f),
		glm::vec3(0.8f, -1.7f, 6.0f)
	};

	const glm::vec3 data_LightColors[] = {
		glm::vec3(200.0f, 200.0f, 200.0f),
		glm::vec3(0.4f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.6f),
		glm::vec3(0.0f, 0.4f, 0.0f)
	};


	Ubpa::Config * DoConfig();
}

#endif // !_DEFINES_H_


