#include <ROOT_PATH.h>

#include <Utility/Image.h>
#include <Utility/GStorage.h>
#include <Utility/InfoLambdaOp.h>
#include <Utility/Config.h>

#include <LOGL/Shader.h>
#include <LOGL/Camera.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "RegisterInput.h"
#include "Defines.h"

using namespace LOGL;
using namespace std;
using namespace Ubpa;
using namespace Define;
//------------

Config * DoConfig();

int main(int argc, char ** argv) {
	Config * config = DoConfig();
	string rootPath = *config->GetStrPtr("RootPath");
	//------------ 窗口
	float ratioWH = (float)val_windowWidth / (float)val_windowHeight;
	string windowTitle = str_Chapter + "/" + str_Subchapter;
	Glfw::GetInstance()->Init(val_windowWidth, val_windowHeight, windowTitle.c_str());
	Glfw::GetInstance()->LockCursor();
	//------------ 模型数据
	glm::vec3 cubePositions[] = {
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
	//------------
	size_t VBO;
	glGenBuffers(1, &VBO);
	size_t VAO;
	glGenVertexArrays(1, &VAO);
	//------------
	glBindVertexArray(VAO);
	//------------
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data_vertices), data_vertices, GL_STATIC_DRAW);
	//------------ 属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//------------ 纹理
	size_t texture[2];
	glGenTextures(2, texture);
	string imgName[2] = {
		rootPath + str_Img_Container,
		rootPath + str_Img_Face
	};
	Image img[2];
	GLenum mode[2] = { GL_RGB, GL_RGBA };
	bool flip[2] = { false, true };
	
	for (size_t i = 0; i < 2; i++) {
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		// 为当前绑定的纹理对象设置环绕、过滤方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		img[i].Load(imgName[i].c_str(), flip[i]);
		if (!img[i].IsValid()) {
			cout << "Failed to load texture[" << imgName[i] << "]\n";
			return 1;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img[i].GetWidth(), img[i].GetHeight(), 0, mode[i], GL_UNSIGNED_BYTE, img[i].GetData());
		// 为当前绑定的纹理自动生成所有需要的多级渐远纹理。
		glGenerateMipmap(GL_TEXTURE_2D);
		img[i].Free();
	}
	GStorage<int>::GetInstance()->Register(str_ChangeTexture, texture[1]);
	//------------ 着色器
	string prefix = rootPath + "/data/shaders/" + str_Chapter + "/" + str_Subchapter + "/";
	string vsF = prefix + str_Subchapter + ".vs";
	string fsF = prefix + str_Subchapter + ".fs";
	Shader shader(vsF.c_str(), fsF.c_str());
	if (!shader.IsValid()) {
		cout << "Shader is not Valid\n";
		return 1;
	}
	//------------ 绑定纹理
	shader.Use();
	shader.SetInt("texture0", 0);
	shader.SetInt("texture1", 1);
	//------------ 矩阵
	float moveSpeed = *config->GetFloatPtr(config_CameraMoveSpeed);
	float rotateSpeed = *config->GetFloatPtr(config_CameraRotateSensitivity);
	Camera mainCamera(ratioWH, moveSpeed, rotateSpeed, glm::vec3(0.0f,0.0f,4.0f));
	GStorage<Camera *>::GetInstance()->Register(str_MainCamera.c_str(), &mainCamera);
	//------------ 输入注册
 	auto registerInputOp = new RegisterInput(false);
	//------------- 时间
	float deltaTime = 0.0f; // 当前帧与上一帧的时间差
	GStorage<float *>::GetInstance()->Register(str_DeltaTime.c_str(), &deltaTime);
	float lastFrame = 0.0f; // 上一帧的时间
	auto timeOp = new LambdaOp([&]() {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	});
	//------------ 渲染
	auto renderOp = new LambdaOp([&]() {
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//------------ 
		glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活纹理单元
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glActiveTexture(GL_TEXTURE1); // 在绑定纹理之前先激活纹理单元
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		//------------
		shader.Use();
		glBindVertexArray(VAO);
		//------------
		shader.SetMat4f("view", glm::value_ptr(mainCamera.GetViewMatrix()));
		shader.SetMat4f("projection", glm::value_ptr(mainCamera.GetProjectionMatrix()));

		for (size_t i = 0; i < 10; i++) {
			glm::mat4 model(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = (float)glfwGetTime() * 50.0f + 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.SetMat4f("model", glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	});
	//-------------
	auto endOp = new LambdaOp ([]() {
		glfwSwapBuffers(Glfw::GetInstance()->GetWindow());
		glfwPollEvents();
	});
	//-------------
	auto opQueue = new OpQueue;
	(*opQueue) << registerInputOp << timeOp << renderOp << endOp;
	//------------
	Glfw::GetInstance()->Run(opQueue);
	//------------
	Glfw::GetInstance()->Terminate();
	return 0;
}

Config * DoConfig() {
	printf("Try to read config.out\n");
	string rootPath;
	Config * config = new Config;
	rootPath = string(ROOT_PATH);
	printf("[1] First Try.\n");
	config->Load(rootPath + "/config/config.out");
	if (!config->IsValid()) {
		rootPath = "../../..";
		printf("[2] Second Try.\n");
		config->Load(rootPath + "/config/config.out");
		if (!config->IsValid()) {
			rootPath = ".";
			printf("[3] Third Try.\n");
			config->Load(rootPath + "/config.out");
			if (!config->IsValid()) {
				printf(
					"ERROR : Three Try All Fail\n"
					"ERROR : RootPath is not valid.\n"
					"Please change config/config.out 's value of RootPath and\n"
					"run exe in correct place( original place or same palce with config.out ).\n");
				exit(1);
			}
		}
	}
	*config->GetStrPtr("RootPath") = rootPath;
	printf("config.out read success\nRootPath is %s\n", config->GetStrPtr("RootPath")->c_str());
	GStorage<Config *>::GetInstance()->Register(str_MainConfig, config);
	return config;
}