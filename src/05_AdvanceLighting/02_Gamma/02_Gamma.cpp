#include <GLFW/Glfw.h>

#include <Utility/GStorage.h>
#include <Utility/LambdaOp.h>
#include <Utility/Config.h>
#include <Utility/Cube.h>
#include <Utility/OpNode.h>

#include <LOGL/Camera.h>
#include <LOGL/Texture.h>
#include <LOGL/VAO.h>
#include <LOGL/FBO.h>
#include <LOGL/Shader.h>

#include <iostream>

#include "Defines.h"
#include "RegisterInput.h"

using namespace LOGL;
using namespace std;
using namespace Ubpa;
using namespace Define;

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
	

	//------------ 模型 . 平面
	VAO VAO_Panel(&(planeVertices[0]), sizeof(planeVertices), { 3,3,2 });
	

	//------------ Gamma 着色器
	string Gamma_vs = rootPath + str_Gamma_vs;
	string Gamma_fs = rootPath + str_Gamma_fs;
	Shader gammaShader(Gamma_vs, Gamma_fs);
	if (!gammaShader.IsValid()) {
		printf("gammaShader load fail\n");
		return 1;
	}
	gammaShader.SetInt("floorTexture", 0);
	gammaShader.UniformBlockBind("CameraMatrixs", 0);
	bool gammaEnable = true;
	GStorage<bool *>::GetInstance()->Register(str_GammaEnable, &gammaEnable);
	gammaShader.SetVec3fs("lightPositions", 4, &data_LightPositions[0][0]);
	gammaShader.SetVec3fs("lightColors", 4, &data_LightColors[0][0]);


	//------------ 纹理
	const int textureNum = 2;
	Texture textures[textureNum];
	string imgPath[textureNum] = {
		rootPath + str_Img_Wood,
		rootPath + str_Img_Wood,
	}; 
	bool flip[textureNum] = { false, false };
	bool gammaCorrection[textureNum] = { false, true };
	for (size_t i = 0; i < textureNum; i++) {
		if (!textures[i].Load(imgPath[i], flip[i], gammaCorrection[i])) {
			printf("Load texture [%s] fail.\n", imgPath[i].c_str());
			return 1;
		}
	}

	//------------ 相机
	float moveSpeed = *config->GetFloatPtr(config_CameraMoveSpeed);
	float rotateSpeed = *config->GetFloatPtr(config_CameraRotateSensitivity);
	Camera mainCamera(ratioWH, moveSpeed, rotateSpeed, glm::vec3(0.0f, 0.0f, 3.0f));
	GStorage<Camera *>::GetInstance()->Register(str_MainCamera.c_str(), &mainCamera);
	  
	//------------ Camera Matrixs Uniform Block
	size_t cameraMatrixsUBO;
	glGenBuffers(1, &cameraMatrixsUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, cameraMatrixsUBO);
	glBufferData(GL_UNIFORM_BUFFER, 2*sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, cameraMatrixsUBO);
	
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

	//------------ 更新相机
	auto cameraMatrixsUBO_Update = new LambdaOp([&]() {
		glBindBuffer(GL_UNIFORM_BUFFER, cameraMatrixsUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(mainCamera.GetViewMatrix()));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(mainCamera.GetProjectionMatrix()));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	});

	//------------ 模型渲染
	gammaShader.SetMat4f("model", glm::mat4(1.0f));
	auto panelOp = new LambdaOp([&]() {
		textures[gammaEnable].Use();
		//textures[0].Use(); 
		gammaShader.Use();
		gammaShader.SetBool("gamma", gammaEnable);
		gammaShader.SetVec3f("viewPos", mainCamera.GetPos());
		VAO_Panel.Use();
		glDrawArrays(GL_TRIANGLES, 0, 6);
	});

	//------------ 渲染操作
	auto renderOp = new OpNode([]() {//init
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	},
		[]() {//end
		glfwSwapBuffers(Glfw::GetInstance()->GetWindow());
		glfwPollEvents();
	});

	(*renderOp) << panelOp;
	
	//------------- 整合
	auto opQueue = new OpQueue;
	(*opQueue) << registerInputOp << timeOp << cameraMatrixsUBO_Update << renderOp;
	
	//------------
	Glfw::GetInstance()->Run(opQueue);
	
	//------------
	Glfw::GetInstance()->Terminate();
	
	return 0;
}

