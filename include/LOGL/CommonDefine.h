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
	
	const std::string str_Img_Container = "/data/textures/container.jpg";
	const std::string str_Img_Container2 = "/data/textures/container2.png";
	const std::string str_Img_Container2_Specular = "/data/textures/container2_specular.png";
	const std::string str_Img_Matrix = "/data/textures/matrix.jpg";
	const std::string str_Img_Face = "/data/textures/awesomeface.png";
	const std::string str_Img_Earth = "/data/textures/earth.jpg";
	const std::string str_Img_Marble = "/data/textures/marble.jpg";
	const std::string str_Img_Grass = "/data/textures/grass.png";
	const std::string str_Img_Window = "/data/textures/window.png";
	const std::string str_Img_Wood = "/data/textures/wood.png";
	
	const std::vector<std::string> str_Vec_Img_Skybox = {
		"/data/textures/skybox/right.jpg",
		"/data/textures/skybox/left.jpg",
		"/data/textures/skybox/top.jpg",
		"/data/textures/skybox/bottom.jpg",
		"/data/textures/skybox/front.jpg",
		"/data/textures/skybox/back.jpg"
	};

	const std::string str_Obj_Nanosuit = "/data/objects/nanosuit/nanosuit.obj";
	
	const std::string str_MainCamera = "MainCamera";
	const std::string str_DeltaTime = "DeltaTime";
	const std::string str_MainConfig = "MainConfig";
	const std::string str_RootPath = "RootPath";
	
	const std::string config_CameraMoveSpeed = "CameraMoveSpeed";
	const std::string config_CameraRotateSensitivity = "CameraRotateSensitivity";

	
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