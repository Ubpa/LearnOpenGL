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
	const std::string str_VsPostfix = ".vs";
	const std::string str_FsPostfix = ".fs";

	const std::string str_ParallaxMap = "parallaxMap";
	const std::string str_ParallaxMap_vs = str_ShaderPrefix + str_ParallaxMap + str_VsPostfix;
	const std::string str_ParallaxMap_fs = str_ShaderPrefix + str_ParallaxMap + str_FsPostfix;

	const std::string str_ImgShow = "imgShow";
	const std::string str_ImgShow_vs = str_ShaderPrefix + str_ImgShow + str_VsPostfix;
	const std::string str_ImgShow_fs = str_ShaderPrefix + str_ImgShow + str_FsPostfix;


	//------------ val
	const size_t val_ShadowWidth = 1024;
	const size_t val_ShadowHeight = 1024;
	const float val_ShadowRatioWH = (float)val_ShadowWidth / (float)val_ShadowHeight;


	//------------ data

	const float data_ScreanVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	const glm::vec3 data_LightPositions[] = {
		glm::vec3(-3.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(3.0f, 0.0f, 0.0f)
	};

	const glm::vec3 data_LightColors[] = {
		glm::vec3(0.25),
		glm::vec3(0.50),
		glm::vec3(0.75),
		glm::vec3(1.00)
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


