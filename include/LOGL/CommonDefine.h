#ifndef _STATIC_CONFIG_H_
#define _STATIC_CONFIG_H_

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Define{
	const size_t val_windowWidth = 1024;
	const size_t val_windowHeight = 576;
	
	const std::string str_Img_Container = "/data/textures/container.jpg";
	const std::string str_Img_Container2 = "/data/textures/container2.png";
	const std::string str_Img_Container2_Specular = "/data/textures/container2_specular.png";
	const std::string str_Img_Matrix = "/data/textures/matrix.jpg";
	const std::string str_Img_Face = "/data/textures/awesomeface.png";
	const std::string str_Img_Earth = "/data/textures/earth.jpg";

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
}

#endif// !_STATIC_CONFIG_H_