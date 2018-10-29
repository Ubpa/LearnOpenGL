#include <GLFW/Glfw.h>

#include <Utility/Image.h>
#include <Utility/GStorage.h>
#include <Utility/LambdaOp.h>
#include <Utility/Config.h>
#include <Utility/Sphere.h>
#include <Utility/Cube.h>
#include <Utility/OpNode.h>

#include <LOGL/Camera.h>
#include <LOGL/Texture.h>
#include <LOGL/Model.h>
#include <LOGL/VAO.h>
#include <LOGL/FBO.h>

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
	

	//------------ BlinnPhong 着色器
	string BlinnPhong_vs = rootPath + str_BlinnPhong_vs;
	string BlinnPhong_fs = rootPath + str_BlinnPhong_fs;
	Shader BlinnPhongShader(BlinnPhong_vs, BlinnPhong_fs);
	if (!BlinnPhongShader.IsValid()) {
		printf("BlinnPhongShader load fail\n");
		return 1;
	}
	BlinnPhongShader.SetInt("floorTexture", 0);
	BlinnPhongShader.UniformBlockBind("CameraMatrixs", 0);
	BlinnPhongShader.SetBool("blinn", true);
	BlinnPhongShader.SetVec3f("lightPos", glm::vec3(0.0f));
	 
	GStorage<Shader *>::GetInstance()->Register(str_BlinnPhong, &BlinnPhongShader);


	//------------ 纹理
	const int textureNum = 1;
	Texture textures[textureNum];
	string imgName[textureNum] = {
		rootPath + str_Img_Wood,
	}; 
	bool flip[textureNum] = { false };
	for (size_t i = 0; i < textureNum; i++) {
		if (!textures[i].Load(imgName[i], flip[i])) {
			printf("Load texture [%s] fail.\n", imgName[i].c_str());
			return 1;
		}
	}

	//------------ 相机
	float moveSpeed = *config->GetFloatPtr(config_CameraMoveSpeed);
	float rotateSpeed = *config->GetFloatPtr(config_CameraRotateSensitivity);
	Camera mainCamera(ratioWH, moveSpeed, rotateSpeed, glm::vec3(0.0f, 0.0f, 4.0f));
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
	BlinnPhongShader.SetMat4f("model", glm::mat4(1.0f));
	auto panelOp = new LambdaOp([&]() {
		textures[0].Use();
		BlinnPhongShader.Use();

		BlinnPhongShader.SetVec3f("viewPos", mainCamera.GetPos());

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

