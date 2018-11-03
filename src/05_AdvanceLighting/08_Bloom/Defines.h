#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <LOGL/CommonDefine.h>
#include <Utility/Config.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Define {
	const std::string str_Chapter = "05_AdvanceLighting";
	const std::string str_Subchapter = "08_Bloom";
	const std::string str_WindowTitle = str_Chapter + "/" + str_Subchapter;

	//------------ shader

	const std::string str_ShaderPrefix = "/data/shaders/" + str_Chapter + "/" + str_Subchapter + "/";

	const std::string str_BloomObj = "bloomObj";
	const std::string str_BloomObj_vs = str_Basic_P3N3T2_vs;
	const std::string str_BloomObj_fs = str_ShaderPrefix + str_BloomObj + str_FsPostfix;

	const std::string str_BloomLight = "bloomLight";
	const std::string str_BloomLight_vs = str_Basic_P3N3T2_vs;
	const std::string str_BloomLight_fs = str_ShaderPrefix + str_BloomLight + str_FsPostfix;

	const std::string str_Blur = "blur";
	const std::string str_Blur_vs = str_Basic_P2T2_vs;
	const std::string str_Blur_fs = str_ShaderPrefix + str_Blur + str_FsPostfix;

	const std::string str_Blend = "blend";
	const std::string str_Blend_vs = str_Basic_P2T2_vs;
	const std::string str_Blend_fs = str_ShaderPrefix + str_Blend + str_FsPostfix;

	//------------ val
	const size_t val_ShadowWidth = 1024;
	const size_t val_ShadowHeight = 1024;
	const float val_ShadowRatioWH = (float)val_ShadowWidth / (float)val_ShadowHeight;


	//------------ data

	const glm::vec3 data_LightPositions[] = {
		glm::vec3(0.0f, 0.5f,  1.5f),
		glm::vec3(-4.0f, 0.5f, -3.0f),
		glm::vec3(3.0f, 0.5f,  1.0f),
		glm::vec3(-.8f,  2.4f, -1.0f)
	};

	const glm::vec3 data_LightColors[] = {
		glm::vec3(2.0f, 2.0f, 2.0f),
		glm::vec3(1.5f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.5f),
		glm::vec3(0.0f, 1.5f, 0.0f)
	};

	const float data_PlaneVertices[] = {
		// positions            // normals         // texcoords
		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
		 25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
	};

	Ubpa::Config * DoConfig();
}

#endif // !_DEFINES_H_


