#ifndef _DEFINE_H_
#define _DEFINE_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

namespace Define {
	const size_t val_windowWidth = 1024;
	const size_t val_windowHeight = 576;

	const std::string str_Chapter = "01_Introduction";
	const std::string str_Subchapter = "07_Camera";
	const std::string str_MainCamera = "mainCamera";
	const std::string str_DeltaTime = "deltaTime";
	const std::string str_ChangeTexture = "changeTexture";
	const std::string str_MainConfig = "MainConfig";

	const std::string str_Img_Container = "/data/textures/container.jpg";
	const std::string str_Img_Face = "/data/textures/awesomeface.png";

	const int textureWarpMode[] = {
		GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_EDGE
	};

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
}

#endif // !_DEFINE_H_


