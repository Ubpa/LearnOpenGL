#include <ROOT_PATH.h>

#include <GLFW/Glfw.h>

#include <Utility/Image.h>
#include <Utility/GStorage.h>
#include <Utility/LambdaOp.h>
#include <Utility/Config.h>
#include <Utility/Sphere.h>
#include <Utility/Cube.h>

#include <LOGL/Camera.h>
#include <LOGL/Texture.h>
#include <LOGL/Model.h>

#include <iostream>

#include "Defines.h"
#include "RegisterInput.h"

using namespace LOGL;
using namespace std;
using namespace Ubpa;
using namespace Define;

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
	Cube cube;
	size_t cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);

	size_t cubeVBO[3];
	glGenBuffers(3, cubeVBO);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, cube.GetVertexArrSize(), cube.GetVertexArr(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, cube.GetNormalArrSize(), cube.GetNormalArr(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO[2]);
	glBufferData(GL_ARRAY_BUFFER, cube.GetTexCoordsArrSize(), cube.GetTexCoordsArr(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(2);

	size_t cubeEBO;
	glGenBuffers(1, &cubeEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube.GetIndexArrSize(), cube.GetIndexArr(), GL_STATIC_DRAW);
	

	//------------ 球
	Sphere sphere = Sphere(30);

	size_t sphereVAO;
	glGenVertexArrays(1, &sphereVAO);
	glBindVertexArray(sphereVAO);

	size_t sphereVertexVBO;
	glGenBuffers(1, &sphereVertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, sphereVertexVBO);
	glBufferData(GL_ARRAY_BUFFER, sphere.GetVertexArrSize(), sphere.GetVertexArr(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	size_t sphereNormalVBO;
	glGenBuffers(1, &sphereNormalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, sphereNormalVBO);
	glBufferData(GL_ARRAY_BUFFER, sphere.GetNormalArrSize(), sphere.GetNormalArr(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(1);

	size_t sphereTexCoordsVBO;
	glGenBuffers(1, &sphereTexCoordsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, sphereTexCoordsVBO);
	glBufferData(GL_ARRAY_BUFFER, sphere.GetTexCoordsArrSize(), sphere.GetTexCoordsArr(), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(2);

	size_t sphereEBO;
	glGenBuffers(1, &sphereEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.GetIndexArrSize(), sphere.GetIndexArr(), GL_STATIC_DRAW);
	
	
	//------------ 灯
	size_t lightVBO;
	glGenBuffers(1, &lightVBO);
	size_t lightVAO;
	glGenVertexArrays(1, &lightVAO);

	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data_vertices), data_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);

	//------------ nanosuit
	Model nanosuit(rootPath + str_Obj_Nanosuit);

	//------------ 纹理
	const int textureNum = 4;
	size_t texture[textureNum];
	string imgName[textureNum] = {
		rootPath + str_Img_Container2,
		rootPath + str_Img_Earth,
		rootPath + str_Img_Container2_Specular,
		rootPath + str_Img_Matrix
	};
	bool flip[4] = { false, true, false, true };
	for (size_t i = 0; i < textureNum; i++) {
		Texture tex(imgName[i].c_str());
		if (!tex.IsValid()) {
			printf("Load texture[%s] fail.\n", imgName[i].c_str());
			return 1;
		}

		texture[i] = tex.GetID();
	}

	//------------ 相机
	float moveSpeed = *config->GetFloatPtr(config_CameraMoveSpeed);
	float rotateSpeed = *config->GetFloatPtr(config_CameraRotateSensitivity);
	Camera mainCamera(ratioWH, moveSpeed, rotateSpeed, glm::vec3(0.0f, 0.0f, 4.0f));
	GStorage<Camera *>::GetInstance()->Register(str_MainCamera.c_str(), &mainCamera);
	
	//------------ nanosuit 着色器
	string nanosuit_vs = rootPath + str_Nanosuit_vs;
	string nanosuit_fs = rootPath + str_Nanosuit_fs;
	Shader nanosuitShader(nanosuit_vs.c_str(), nanosuit_fs.c_str());
	if (!nanosuitShader.IsValid()) {
		cout << "nanosuitShader is not Valid\n";
		return 1;
	}

	//------------ 光源着色器
	string light_vs = rootPath + str_Light_vs;
	string light_fs = rootPath + str_Light_fs;
	Shader lightShader(light_vs.c_str(), light_fs.c_str());
	if (!lightShader.IsValid()) {
		cout << "lightShader is not Valid\n";
		return 1;
	}
	lightShader.Use();
	lightShader.SetVec3f("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
	
	
	//------------ 光照着色器
	string lighting_vs = rootPath + str_Lighting_vs;
	string lighting_fs = rootPath + str_Lighting_fs;
	Shader lightingShader(lighting_vs.c_str(), lighting_fs.c_str());
	if (!lightingShader.IsValid()) {
		cout << "lightingShader is not Valid\n";
		return 1;
	} 
	
	
	//材质 
	lightingShader.Use();
	lightingShader.SetInt("material.diffuse", 0);
	lightingShader.SetInt("material.specular", 1);
	lightingShader.SetInt("material.emission", 2);
	lightingShader.SetFloat("material.shininess", 32.0f);
	lightingShader.SetFloat("material.constant", 1.0f);
	lightingShader.SetFloat("material.linear", 0.09f);
	lightingShader.SetFloat("material.quadratic", 0.032f);

	nanosuitShader.Use();
	nanosuitShader.SetFloat("material.shininess", 32.0f);

	//光源
	Shader * pShaders[2] = { &lightingShader, &nanosuitShader };
	for (size_t i = 0; i < 2; i++) {
		pShaders[i]->Use();
		// directional light
		pShaders[i]->SetVec3f("dirLight.direction", -0.2f, -1.0f, -0.3f);
		pShaders[i]->SetVec3f("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		pShaders[i]->SetVec3f("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		pShaders[i]->SetVec3f("dirLight.specular", 0.5f, 0.5f, 0.5f);
		// point light 1
		pShaders[i]->SetVec3f("pointLights[0].position", pointLightPositions[0]);
		pShaders[i]->SetVec3f("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		pShaders[i]->SetVec3f("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		pShaders[i]->SetVec3f("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		pShaders[i]->SetFloat("pointLights[0].constant", 1.0f);
		pShaders[i]->SetFloat("pointLights[0].linear", 0.09);
		pShaders[i]->SetFloat("pointLights[0].quadratic", 0.032);
		// point light 2
		pShaders[i]->SetVec3f("pointLights[1].position", pointLightPositions[1]);
		pShaders[i]->SetVec3f("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		pShaders[i]->SetVec3f("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		pShaders[i]->SetVec3f("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		pShaders[i]->SetFloat("pointLights[1].constant", 1.0f);
		pShaders[i]->SetFloat("pointLights[1].linear", 0.09);
		pShaders[i]->SetFloat("pointLights[1].quadratic", 0.032);
		// point light 3
		pShaders[i]->SetVec3f("pointLights[2].position", pointLightPositions[2]);
		pShaders[i]->SetVec3f("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		pShaders[i]->SetVec3f("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		pShaders[i]->SetVec3f("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		pShaders[i]->SetFloat("pointLights[2].constant", 1.0f);
		pShaders[i]->SetFloat("pointLights[2].linear", 0.09);
		pShaders[i]->SetFloat("pointLights[2].quadratic", 0.032);
		// point light 4
		pShaders[i]->SetVec3f("pointLights[3].position", pointLightPositions[3]);
		pShaders[i]->SetVec3f("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		pShaders[i]->SetVec3f("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		pShaders[i]->SetVec3f("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		pShaders[i]->SetFloat("pointLights[3].constant", 1.0f);
		pShaders[i]->SetFloat("pointLights[3].linear", 0.09);
		pShaders[i]->SetFloat("pointLights[3].quadratic", 0.032);
		// spotLight
		pShaders[i]->SetVec3f("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		pShaders[i]->SetVec3f("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		pShaders[i]->SetVec3f("spotLight.specular", 1.0f, 1.0f, 1.0f);
		pShaders[i]->SetFloat("spotLight.constant", 1.0f);
		pShaders[i]->SetFloat("spotLight.linear", 0.09);
		pShaders[i]->SetFloat("spotLight.quadratic", 0.032);
		pShaders[i]->SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		pShaders[i]->SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
	}

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
		glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	});
	
	//------------ 渲染正方体模型
	auto cubeOp = new LambdaOp([&]() {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texture[3]);

		lightingShader.Use();

		lightingShader.SetVec3f("viewPos", mainCamera.GetPos());
		lightingShader.SetVec3f("spotLight.position", mainCamera.GetPos());
		lightingShader.SetVec3f("spotLight.direction", mainCamera.GetFront());

		lightingShader.SetMat4f("view", mainCamera.GetViewMatrix());
		lightingShader.SetMat4f("projection", mainCamera.GetProjectionMatrix());
		glBindVertexArray(cubeVAO);
		for (size_t i = 0; i < 10; i++)
		{
			glm::mat4 model(1.0f);
			float t = glfwGetTime();
			model = glm::translate(model, cubePositions[i]);

			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)*(sinf(t)*0.5f) + 1.0f);

			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			model = glm::rotate(model, 1.4f * t, glm::vec3(1.0f, 1.0f, 1.0f));

			lightingShader.SetMat4f("model", model);

			glDrawElements(GL_TRIANGLES, cube.GetTriNum() * 3, GL_UNSIGNED_INT, NULL);
		}
	}); 

	//------------ 渲染球体模型
	auto sphereOp = new LambdaOp([&]() {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texture[2]);

		lightingShader.Use();

		lightingShader.SetVec3f("viewPos", mainCamera.GetPos());
		lightingShader.SetVec3f("spotLight.position", mainCamera.GetPos());
		lightingShader.SetVec3f("spotLight.direction", mainCamera.GetFront());

		lightingShader.SetMat4f("view", mainCamera.GetViewMatrix());
		lightingShader.SetMat4f("projection", mainCamera.GetProjectionMatrix());
		glm::mat4 model = glm::mat4(1.0f);
		float t = glfwGetTime();
		model = glm::translate(model, glm::vec3(4.0f, -0.5f + sinf(2*t), -1.5f));
		lightingShader.SetMat4f("model", model);

		glBindVertexArray(sphereVAO);
		glDrawElements(GL_TRIANGLES, sphere.GetTriNum() * 3, GL_UNSIGNED_INT, NULL);
	});

	//------------ 渲染 nanosuit
	auto nanosuitOp = new LambdaOp([&]() {
		nanosuitShader.Use();

		nanosuitShader.SetVec3f("viewPos", mainCamera.GetPos());
		nanosuitShader.SetVec3f("spotLight.position", mainCamera.GetPos());
		nanosuitShader.SetVec3f("spotLight.direction", mainCamera.GetFront());
		// view/projection transformations
		nanosuitShader.SetMat4f("projection", mainCamera.GetProjectionMatrix());
		nanosuitShader.SetMat4f("view", mainCamera.GetViewMatrix());

		for (size_t i = 0; i < 10; i++)
		{
			glm::mat4 model(1.0f);
			float t = glfwGetTime();
			model = glm::translate(model, glm::vec3(5.0f, 0.0f, -5.0f)); // translate it down so it's at the center of the scene
			model = glm::translate(model, cubePositions[i]);

			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)*(sinf(t)*0.5f) + 1.0f);
			model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down

			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			model = glm::rotate(model, 1.4f * t, glm::vec3(1.0f, 1.0f, 1.0f));

			nanosuitShader.SetMat4f("model", model);

			nanosuit.Draw(nanosuitShader, "material.");
		}
	});

	auto lightOp = new LambdaOp([&]() {
		lightShader.Use();
		lightShader.SetMat4f("view", mainCamera.GetViewMatrix());
		lightShader.SetMat4f("projection", mainCamera.GetProjectionMatrix());
		
		for (size_t i = 0; i < 4; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
			lightShader.SetMat4f("model", model);
			
			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	});

	//------------ 渲染
	auto renderOp = new OpQueue;
	(*renderOp) << clearOp << cubeOp << sphereOp << nanosuitOp << lightOp;
	
	//------------- 末尾
	auto endOp = new LambdaOp([]() {
		glfwSwapBuffers(Glfw::GetInstance()->GetWindow());
		glfwPollEvents();
	});
	
	//------------- 整合
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