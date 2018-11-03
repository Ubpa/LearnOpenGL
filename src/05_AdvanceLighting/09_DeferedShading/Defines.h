#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <LOGL/CommonDefine.h>
#include <Utility/Config.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Define {
	const std::string str_Chapter = "05_AdvanceLighting";
	const std::string str_Subchapter = "09_DeferedShading";
	const std::string str_WindowTitle = str_Chapter + "/" + str_Subchapter;

	//------------ shader
	const std::string str_ShaderPrefix = "/data/shaders/" + str_Chapter + "/" + str_Subchapter + "/";

	const std::string str_GBuffer = "gBuffer";
	const std::string str_GBuffer_vs = str_Basic_P3N3T2_vs;
	const std::string str_GBuffer_fs = str_ShaderPrefix + str_GBuffer + str_FsPostfix;

	const std::string str_DeferedShading = "deferedShading";
	const std::string str_DeferedShading_vs = str_Basic_P2T2_vs;
	const std::string str_DeferedShading_fs = str_ShaderPrefix + str_DeferedShading + str_FsPostfix;

	const std::string str_LightBox_vs = str_ColorShow_vs;
	const std::string str_LightBox_fs = str_ColorShow_fs;


	//------------ data
	const size_t val_NanosuitNum = 9;
	const glm::vec3 data_NanosuitPositions[val_NanosuitNum] = {
		glm::vec3(-3.0,  -3.0, -3.0),
		glm::vec3(0.0,  -3.0, -3.0),
		glm::vec3(3.0,  -3.0, -3.0),
		glm::vec3(-3.0,  -3.0,  0.0),
		glm::vec3(0.0,  -3.0,  0.0),
		glm::vec3(3.0,  -3.0,  0.0),
		glm::vec3(-3.0,  -3.0,  3.0),
		glm::vec3(0.0,  -3.0,  3.0),
		glm::vec3(3.0,  -3.0,  3.0),
	};

	Ubpa::Config * DoConfig();
}

#endif // !_DEFINES_H_


