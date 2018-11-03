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
	
	//------------ 平面
	size_t panelVAO;
	glGenVertexArrays(1, &panelVAO);
	glBindVertexArray(panelVAO);
	size_t panelVBO;
	glGenBuffers(1, &panelVBO);
	glBindBuffer(GL_ARRAY_BUFFER, panelVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
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

	//------------ trasparent panel
	size_t transparentVBO;
	glGenBuffers(1, &transparentVBO);
	size_t transparentVAO;
	glGenVertexArrays(1, &transparentVAO);

	glBindVertexArray(transparentVAO);

	glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//------------ 纹理
	const int textureNum = 7;
	size_t texture[textureNum];
	string imgName[textureNum] = {
		rootPath + str_Img_Container2,
		rootPath + str_Img_Earth,
		rootPath + str_Img_Container2_Specular,
		rootPath + str_Img_Matrix,
		rootPath + str_Img_Marble,
		rootPath + str_Img_Grass,
		rootPath + str_Img_Window
	};
	bool flip[textureNum] = { false, true, false, true, false, true, false };
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

	//------------ transparent 着色器
	string transparent_vs = rootPath + str_Transparent_vs;
	string transparent_fs = rootPath + str_Transparent_fs;
	Shader transparentShader(transparent_vs.c_str(), transparent_fs.c_str());
	if (!transparentShader.IsValid()) {
		cout << "transparentShader is not Valid\n";
		return 1;
	}
	transparentShader.Use();
	transparentShader.SetInt("texture1", 0);

	//------------ 单色着色器
	string singleColor_vs = rootPath + str_SingleColor_vs;
	string singleColor_fs = rootPath + str_SingleColor_fs;
	Shader singleColorShader(singleColor_vs.c_str(), singleColor_fs.c_str());
	if (!singleColorShader.IsValid()) {
		cout << "singleColorShader is not Valid\n";
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
		pShaders[i]->SetFloat("pointLights[0].linear", 0.09f);
		pShaders[i]->SetFloat("pointLights[0].quadratic", 0.032f);
		// point light 2
		pShaders[i]->SetVec3f("pointLights[1].position", pointLightPositions[1]);
		pShaders[i]->SetVec3f("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		pShaders[i]->SetVec3f("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		pShaders[i]->SetVec3f("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		pShaders[i]->SetFloat("pointLights[1].constant", 1.0f);
		pShaders[i]->SetFloat("pointLights[1].linear", 0.09f);
		pShaders[i]->SetFloat("pointLights[1].quadratic", 0.032f);
		// point light 3
		pShaders[i]->SetVec3f("pointLights[2].position", pointLightPositions[2]);
		pShaders[i]->SetVec3f("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		pShaders[i]->SetVec3f("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		pShaders[i]->SetVec3f("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		pShaders[i]->SetFloat("pointLights[2].constant", 1.0f);
		pShaders[i]->SetFloat("pointLights[2].linear", 0.09f);
		pShaders[i]->SetFloat("pointLights[2].quadratic", 0.032f);
		// point light 4
		pShaders[i]->SetVec3f("pointLights[3].position", pointLightPositions[3]);
		pShaders[i]->SetVec3f("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		pShaders[i]->SetVec3f("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		pShaders[i]->SetVec3f("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		pShaders[i]->SetFloat("pointLights[3].constant", 1.0f);
		pShaders[i]->SetFloat("pointLights[3].linear", 0.09f);
		pShaders[i]->SetFloat("pointLights[3].quadratic", 0.032f);
		// spotLight
		pShaders[i]->SetVec3f("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		pShaders[i]->SetVec3f("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		pShaders[i]->SetVec3f("spotLight.specular", 1.0f, 1.0f, 1.0f);
		pShaders[i]->SetFloat("spotLight.constant", 1.0f);
		pShaders[i]->SetFloat("spotLight.linear", 0.09f);
		pShaders[i]->SetFloat("spotLight.quadratic", 0.032f);
		pShaders[i]->SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		pShaders[i]->SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
	}

	//------------ nanosuit
	Model nanosuit(rootPath + str_Obj_Nanosuit);


	auto initOp = new LambdaOp([]() {
		glDepthFunc(GL_LESS);
		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glClearStencil(0xFF);
		glEnable(GL_BLEND);
		glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
	}, false);

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	});
	
	auto panelOp = new LambdaOp([&]() {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[4]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, 0);
		lightingShader.Use();

		lightingShader.SetVec3f("viewPos", mainCamera.GetPos());
		lightingShader.SetVec3f("spotLight.position", mainCamera.GetPos());
		lightingShader.SetVec3f("spotLight.direction", mainCamera.GetFront());

		lightingShader.SetMat4f("view", mainCamera.GetViewMatrix());
		lightingShader.SetMat4f("projection", mainCamera.GetProjectionMatrix());
		lightingShader.SetMat4f("model", glm::mat4(1.0f));
		
		glBindVertexArray(panelVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
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


		singleColorShader.Use();

		singleColorShader.SetMat4f("view", mainCamera.GetViewMatrix());
		singleColorShader.SetMat4f("projection", mainCamera.GetProjectionMatrix());

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);

		glBindVertexArray(cubeVAO);
		glEnable(GL_STENCIL_TEST);
		for (size_t i = 0; i < 10; i++)
		{
			glm::mat4 model(1.0f);
			float t = glfwGetTime();
			model = glm::translate(model, cubePositions[i]);

			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			model = glm::rotate(model, 1.4f * t, glm::vec3(1.0f, 1.0f, 1.0f));

			glm::mat4 border = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)*(sinf(t)*0.5f) + 1.1f);
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)*(sinf(t)*0.5f) + 1.0f);

			lightingShader.Use();
			lightingShader.SetMat4f("model", model);

			glEnable(GL_DEPTH_TEST);
			glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
			glStencilFunc(GL_ALWAYS, 1, 0xFF); // 所有的片段都应该更新模板缓冲
			glStencilMask(0xFF); // 启用模板缓冲写入

			glDrawElements(GL_TRIANGLES, cube.GetTriNum() * 3, GL_UNSIGNED_INT, NULL);


			singleColorShader.Use();
			singleColorShader.SetMat4f("model", border);

			glStencilOp(GL_KEEP, GL_ZERO, GL_ZERO);
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0xFF);
			glDisable(GL_DEPTH_TEST);

			glDrawElements(GL_TRIANGLES, cube.GetTriNum() * 3, GL_UNSIGNED_INT, NULL);
		}

		glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilMask(0xFF); // 启用模板缓冲写入
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
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

			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)*(sinf(0.1f*i+t)*0.5f) + 1.0f);
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

	auto grassOp = new LambdaOp([&]() {
		transparentShader.Use();
		transparentShader.SetMat4f("view", mainCamera.GetViewMatrix());
		transparentShader.SetMat4f("projection", mainCamera.GetProjectionMatrix());
		glBindVertexArray(transparentVAO);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[5]);
		
		for (size_t i = 0; i < 5; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, vegetationp[i]);
			transparentShader.SetMat4f("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture[6]);

		std::map<float, glm::vec3> sorted;
		for (size_t i = 0; i < 5; i++)
		{
			glm::vec3 windowPos = vegetationp[i] + glm::vec3(-5.0f, 0, 0.2f);
			float distance = glm::length(mainCamera.GetPos() - windowPos);
			sorted[distance] = windowPos;
		}

		for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, it->second);
			transparentShader.SetMat4f("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	});

	//------------ 渲染
	auto renderOp = new OpQueue;
	(*renderOp) << clearOp << cubeOp << panelOp << sphereOp << nanosuitOp << lightOp << grassOp;
	
	//------------- 末尾
	auto endOp = new LambdaOp([]() {
		glfwSwapBuffers(Glfw::GetInstance()->GetWindow());
		glfwPollEvents(); 
	});
	
	//------------- 整合
	auto opQueue = new OpQueue;
	(*opQueue) << initOp << registerInputOp << timeOp << renderOp << endOp;
	
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