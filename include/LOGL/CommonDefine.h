#ifndef _COMMOM_DEFINE_H_
#define _COMMOM_DEFINE_H_

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Define{
	const size_t val_windowWidth = 1280;
	const size_t val_windowHeight = 720;
	const float val_RatioWH = (float)val_windowWidth / (float)val_windowHeight;
	const float PI = 3.1415926;
	
	const std::string str_Img_Prefix = "/data/textures/";
	const std::string str_Img_Container = str_Img_Prefix + "container.jpg";
	const std::string str_Img_Container2 = str_Img_Prefix + "container2.png";
	const std::string str_Img_Container2_Specular = str_Img_Prefix + "container2_specular.png";
	const std::string str_Img_Matrix = str_Img_Prefix + "matrix.jpg";
	const std::string str_Img_Face = str_Img_Prefix + "awesomeface.png";
	const std::string str_Img_Earth = str_Img_Prefix + "earth.jpg";
	const std::string str_Img_Marble = str_Img_Prefix + "marble.jpg";
	const std::string str_Img_Grass = str_Img_Prefix + "grass.png";
	const std::string str_Img_Window = str_Img_Prefix + "window.png";
	const std::string str_Img_Wood = str_Img_Prefix + "wood.png";
	const std::string str_Img_Brickwall = str_Img_Prefix + "brickwall.jpg";
	const std::string str_Img_BrickwallNormal = str_Img_Prefix + "brickwall_normal.jpg";
	const std::string str_Img_Bricks2 = str_Img_Prefix + "bricks2.jpg";
	const std::string str_Img_Bricks2_disp = str_Img_Prefix + "bricks2_disp.jpg";
	const std::string str_Img_Bricks2_normal = str_Img_Prefix + "bricks2_normal.jpg";

	
	const std::vector<std::string> str_Vec_Img_Skybox = {
		str_Img_Prefix + "skybox/right.jpg",
		str_Img_Prefix + "skybox/left.jpg",
		str_Img_Prefix + "skybox/top.jpg",
		str_Img_Prefix + "skybox/bottom.jpg",
		str_Img_Prefix + "skybox/front.jpg",
		str_Img_Prefix + "skybox/back.jpg"
	};

	const std::string str_Obj_Nanosuit = "/data/objects/nanosuit/nanosuit.obj";
	
	const std::string str_MainCamera = "MainCamera";
	const std::string str_DeltaTime = "DeltaTime";
	const std::string str_MainConfig = "MainConfig";
	const std::string str_RootPath = "RootPath";
	
	const std::string config_CameraMoveSpeed = "CameraMoveSpeed";
	const std::string config_CameraRotateSensitivity = "CameraRotateSensitivity";

	//------------ common shader
	const std::string str_ShadersCommon_Prefix = "/data/shaders/Common/";
	const std::string str_VsPostfix = ".vs";
	const std::string str_FsPostfix = ".fs";

	const std::string str_Basic_P3N3T2_vs = str_ShadersCommon_Prefix + "basic_P3N3T2" + str_VsPostfix;
	const std::string str_Basic_P3T2_vs = str_ShadersCommon_Prefix + "basic_P3T2" + str_VsPostfix;
	const std::string str_Basic_P2T2_vs = str_ShadersCommon_Prefix + "basic_P2T2" + str_VsPostfix;
	const std::string str_Basic_P3_vs = str_ShadersCommon_Prefix + "basic_P3" + str_VsPostfix;

	const std::string str_Basic_T2_fs = str_ShadersCommon_Prefix + "basic_T2" + str_FsPostfix;

	const std::string str_ImgShow_vs = str_Basic_P3T2_vs;
	const std::string str_ImgShow_fs = str_ShadersCommon_Prefix + "basic_T2" + str_FsPostfix;

	const float data_ScreenVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};
	

	//position, TexCoords
	const float data_vertices[] = {
		-0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,  1.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,  1.5f,  1.5f,
		 0.5f,  0.5f, -0.5f,  1.5f,  1.5f,
		-0.5f,  0.5f, -0.5f, -0.5f,  1.5f,
		-0.5f, -0.5f, -0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,  1.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,  1.5f,  1.5f,
		 0.5f,  0.5f,  0.5f,  1.5f,  1.5f,
		-0.5f,  0.5f,  0.5f, -0.5f,  1.5f,
		-0.5f, -0.5f,  0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f,  0.5f,  1.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,  1.5f,  1.5f,
		-0.5f, -0.5f, -0.5f, -0.5f,  1.5f,
		-0.5f, -0.5f, -0.5f, -0.5f,  1.5f,
		-0.5f, -0.5f,  0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f,  0.5f,  1.5f, -0.5f,

		 0.5f,  0.5f,  0.5f,  1.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,  1.5f,  1.5f,
		 0.5f, -0.5f, -0.5f, -0.5f,  1.5f,
		 0.5f, -0.5f, -0.5f, -0.5f,  1.5f,
		 0.5f, -0.5f,  0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,  1.5f, -0.5f,

		-0.5f, -0.5f, -0.5f, -0.5f,  1.5f,
		 0.5f, -0.5f, -0.5f,  1.5f,  1.5f,
		 0.5f, -0.5f,  0.5f,  1.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,  1.5f, -0.5f,
		-0.5f, -0.5f,  0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f, -0.5f,  1.5f,

		-0.5f,  0.5f, -0.5f, -0.5f,  1.5f,
		 0.5f,  0.5f, -0.5f,  1.5f,  1.5f,
		 0.5f,  0.5f,  0.5f,  1.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,  1.5f, -0.5f,
		-0.5f,  0.5f,  0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f, -0.5f,  1.5f,
	};

	//position, normal
	const float cubeData[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	const glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	const float planeVertices[] = {
		// positions          //noraml          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
		 5.0f, -0.5f,  5.0f,  0.0f, 1.0f, 0.0f, 2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 1.0f, 0.0f, 0.0f, 2.0f,

		 5.0f, -0.5f,  5.0f,  0.0f, 1.0f, 0.0f, 2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 1.0f, 0.0f, 0.0f, 2.0f,
		 5.0f, -0.5f, -5.0f,  0.0f, 1.0f, 0.0f, 2.0f, 2.0f
	};

	const float transparentVertices[] = {
		// positions         //normal          // texture Coords (swapped y coordinates because texture is flipped upside down)
		0.0f,  0.5f,  0.0f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f,
		0.0f, -0.5f,  0.0f,  0.0f, 0.0f, 1.0f, 0.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  0.0f, 0.0f, 1.0f, 1.0f,  1.0f,

		0.0f,  0.5f,  0.0f,  0.0f, 0.0f, 1.0f, 0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  0.0f, 0.0f, 1.0f, 1.0f,  1.0f,
		1.0f,  0.5f,  0.0f,  0.0f, 0.0f, 1.0f, 1.0f,  0.0f
	};
}

#endif// !_COMMOM_DEFINE_H_