#include <ROOT_PATH.h>

#include <Utility/Image.h>
#include <Utility/GStorage.h>
#include <Utility/LambdaOp.h>
#include <Utility/Config.h>

#include <LOGL/Shader.h>
#include <LOGL/Camera.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Defines.h"
#include "RegisterInput.h"

using namespace LOGL;
using namespace std;
using namespace Ubpa;
using namespace Define;
//------------

Config * DoConfig();

int main(int argc, char ** argv) {
	Config * config = DoConfig();
	string rootPath = *config->GetStrPtr(str_RootPath);
	GStorage<Config *>::GetInstance()->Register(str_MainConfig, config);
	GStorage<string>::GetInstance()->Register(str_RootPath, rootPath);
	//------------ 窗口
	float ratioWH = (float)val_windowWidth / (float)val_windowHeight;
	string windowTitle = str_Chapter + "/" + str_Subchapter;
	Glfw::GetInstance()->Init(val_windowWidth, val_windowHeight, windowTitle.c_str());
	Glfw::GetInstance()->LockCursor();
	//------------ 正方体
	size_t VBO;
	glGenBuffers(1, &VBO);
	size_t VAO;
	glGenVertexArrays(1, &VAO);
	
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data_vertices), data_vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);

	//------------ 灯
	size_t lightVBO;
	glGenBuffers(1, &lightVBO);
	size_t lightVAO;
	glGenVertexArrays(1, &lightVAO);

	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data_vertices), data_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);

	//------------ 光源着色器
	string light_vs = rootPath + str_Light_vs;
	string light_fs = rootPath + str_Light_fs;
	Shader lightShader(light_vs.c_str(), light_fs.c_str());
	if (!lightShader.IsValid()) {
		cout << "lightShader is not Valid\n";
		return 1;
	}
	//------------ 光照着色器
	string lighting_vs = rootPath + str_Lighting_vs;
	string lighting_fs = rootPath + str_Lighting_fs;
	Shader lightingShader(lighting_vs.c_str(), lighting_fs.c_str());
	if (!lightingShader.IsValid()) {
		cout << "lightingShader is not Valid\n";
		return 1;
	}
	lightingShader.Use();
	lightingShader.SetVec3f("objectColor", 1.0f, 0.5f, 0.31f);
	lightingShader.SetVec3f("lightColor", 1.0f, 1.0f, 1.0f);
	//------------ 相机
	float moveSpeed = *config->GetFloatPtr(config_CameraMoveSpeed);
	float rotateSpeed = *config->GetFloatPtr(config_CameraRotateSensitivity);
	Camera mainCamera(ratioWH, moveSpeed, rotateSpeed, glm::vec3(0.0f, 0.0f, 4.0f));
	GStorage<Camera *>::GetInstance()->Register(str_MainCamera.c_str(), &mainCamera);
	//------------ 光源模型
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);
	lightModel = glm::scale(lightModel, glm::vec3(0.2f));
	//------------ 输入
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
	//------------ 清除
	auto clearOp = new LambdaOp([]() {
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	});
	//------------ 渲染正方体模型
	auto cubeOp = new LambdaOp([&]() {
		lightingShader.Use();
		glBindVertexArray(VAO);
		lightingShader.SetMat4f("view", glm::value_ptr(mainCamera.GetViewMatrix()));
		lightingShader.SetMat4f("projection", glm::value_ptr(mainCamera.GetProjectionMatrix()));
		lightingShader.SetMat4f("model", glm::value_ptr(glm::mat4(1.0f)));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	});

	auto lightOp = new LambdaOp([&]() {
		lightShader.Use();
		glBindVertexArray(lightVAO);
		lightShader.SetMat4f("view", glm::value_ptr(mainCamera.GetViewMatrix()));
		lightShader.SetMat4f("projection", glm::value_ptr(mainCamera.GetProjectionMatrix()));
		lightShader.SetMat4f("model", glm::value_ptr(lightModel));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	});

	//------------ 渲染
	auto renderOp = new OpQueue;
	(*renderOp) << clearOp << cubeOp << lightOp;
	//------------- 末尾
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
	GStorage<Config *>::GetInstance()->Register(str_MainCamera, config);
	return config;
}