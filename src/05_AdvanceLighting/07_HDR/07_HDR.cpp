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
	Glfw::GetInstance()->Init(val_windowWidth, val_windowHeight, str_WindowTitle);
	Glfw::GetInstance()->LockCursor();


	//------------ 模型 . Cube
	Cube cube;
	vector<VAO::VBO_DataPatch> cube_Vec_VBO_Data_Patch = {
		{cube.GetVertexArr(), cube.GetVertexArrSize(), 3},
		{cube.GetNormalArr(), cube.GetNormalArrSize(), 3},
		{cube.GetTexCoordsArr(), cube.GetTexCoordsArrSize(), 2},
	};
	VAO VAO_Cube(cube_Vec_VBO_Data_Patch, cube.GetIndexArr(), cube.GetIndexArrSize());

	//------------ 模型 . imgShowCube
	vector<VAO::VBO_DataPatch> imgShowCube_Vec_VBO_Data_Patch = {
		{cube.GetVertexArr(), cube.GetVertexArrSize(), 3},
		{cube.GetTexCoordsArr(), cube.GetTexCoordsArrSize(), 2},
	};
	VAO VAO_ImgShowCube(imgShowCube_Vec_VBO_Data_Patch, cube.GetIndexArr(), cube.GetIndexArrSize());


	//------------ 模型 . Screen
	VAO VAO_Screen(&(data_ScreenVertices[0]), sizeof(data_ScreenVertices), { 2,2 });
	
	//------------ ParallaxMap 着色器
	string HDR_vs = rootPath + str_HDR_vs;
	string HDR_fs = rootPath + str_HDR_fs;
	Shader HDRShader(HDR_vs, HDR_fs);
	if (!HDRShader.IsValid()) {
		printf("ERROR: HDRShader load fail\n"); 
		return 1;
	}
	HDRShader.SetInt("hdrBuffer", 0);
	int mode = 0;
	float exposure = 1.0;
	GStorage<int *>::GetInstance()->Register("int_ptr_mode", &mode);
	GStorage<float *>::GetInstance()->Register("float_ptr_exposure", &exposure);

	//------------ ImgShow 着色器
	string imgShow_vs = rootPath + str_ImgShow_vs;
	string imgShow_fs = rootPath + str_ImgShow_fs;
	Shader imgShowShader(imgShow_vs, imgShow_fs);
	if (!imgShowShader.IsValid()) {
		printf("ERROR: imgShowShader load fail\n");
		return 1; 
	}
	imgShowShader.SetInt("texture0", 0);
	imgShowShader.UniformBlockBind("CameraMatrixs", 0);

	//------------ Lighting 着色器
	string lighting_vs = rootPath + str_Lighting_vs;
	string lighting_fs = rootPath + str_Lighting_fs;
	Shader lightingShader(lighting_vs, lighting_fs);
	if (!lightingShader.IsValid()) {
		printf("ERROR: lightingShader load fail\n");
		return 1;
	}
	lightingShader.SetInt("diffuseTexture", 0);
	lightingShader.UniformBlockBind("CameraMatrixs", 0);
	lightingShader.SetBool("inverse_normals", true);
	const size_t lightNum = 4;
	for (size_t i = 0; i < lightNum; i++) {
		lightingShader.SetVec3f("lights[" + to_string(i) + "].Position", data_LightPositions[i]);
		lightingShader.SetVec3f("lights[" + to_string(i) + "].Color", data_LightColors[i]);
	}
	lightingShader.SetInt("lightNum", lightNum);

	//------------ 纹理
	const int textureNum = 1;
	Texture textures[textureNum];
	string imgPath[textureNum] = {
		rootPath + str_Img_Wood,
	};
	bool flip[textureNum] = { true };
	for (size_t i = 0; i < textureNum; i++) {
		if (!textures[i].Load(imgPath[i], flip[i])) {
			printf("ERROR: Load texture [%s] fail.\n", imgPath[i].c_str());
			return 1;
		}
	}

	//------------ 相机
	float moveSpeed = *config->GetFloatPtr(config_CameraMoveSpeed);
	float rotateSpeed = *config->GetFloatPtr(config_CameraRotateSensitivity);
	Camera mainCamera(val_RatioWH, moveSpeed, rotateSpeed, glm::vec3(0.0f, 0.0f, -2.49f), 0.1f, 100.0f, glm::vec3(0, 1.0f, 0.0f), 90.0f,-10.0f);
	GStorage<Camera *>::GetInstance()->Register(str_MainCamera.c_str(), &mainCamera);
	
	//------------ Camera Matrixs UBO
	size_t cameraMatrixsUBO;
	glGenBuffers(1, &cameraMatrixsUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, cameraMatrixsUBO);
	glBufferData(GL_UNIFORM_BUFFER, 2*sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, cameraMatrixsUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);


	//------------ HDR帧缓冲
	FBO FBO_HDR(val_windowWidth, val_windowHeight, FBO::ENUM_TYPE_RGBF1_DEPTH);

	//------------ 输入
	auto registerInputOp = new RegisterInput(false);
	 
	//------------- 时间
	float deltaTime = 0.0f; // 当前帧与上一帧的时间差
	GStorage<float *>::GetInstance()->Register(str_DeltaTime.c_str(), &deltaTime);
	float lastFrame = 0.0f; // 上一帧的时间
	auto timeUpdate = new LambdaOp([&]() {
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

	auto updateOpQueue = new OpQueue;
	(*updateOpQueue) << timeUpdate << cameraMatrixsUBO_Update;


	//------------ 模型场景
	auto tunnelOp = new LambdaOp([&]() {
		textures[0].Use();
		
		lightingShader.SetVec3f("viewPos", mainCamera.GetPos());

		// render tunnel
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 25.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 55.0f));
		lightingShader.SetMat4f("model", model);
		VAO_Cube.Draw();
	});

	auto imgShowOp = new LambdaOp([&]() {
		glm::mat4 model(1.0f);
		
		for (size_t i = 0; i < textureNum; i++) {
			model = glm::translate(model, glm::vec3(1.99f, 0, 0));
			imgShowShader.SetMat4f("model", model);
			textures[i].Use();
			VAO_ImgShowCube.Draw();
		}
	});

	auto screenOp = new LambdaOp([&]() {
		FBO_HDR.GetColorTexture().Use();
		HDRShader.Use();
		HDRShader.SetInt("mode", mode);
		HDRShader.SetFloat("exposure", exposure);
		VAO_Screen.Draw();
	});

	//------------ 渲染操作
	auto hdrOp = new OpNode([&]() {
		FBO_HDR.Use();
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}, []() {
		FBO::UseDefault();
	});
	(*hdrOp) << tunnelOp << imgShowOp;

	auto renderOp = new OpNode([]() {//init
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}, []() {//end
		glfwSwapBuffers(Glfw::GetInstance()->GetWindow());
		glfwPollEvents();
	});

	(*renderOp) << screenOp;
	
	//------------- 整合
	auto opQueue = new OpQueue;
	(*opQueue) << registerInputOp << updateOpQueue << hdrOp << renderOp;
	
	//------------
	Glfw::GetInstance()->Run(opQueue);
	
	//------------
	Glfw::GetInstance()->Terminate();
	
	return 0;
}

