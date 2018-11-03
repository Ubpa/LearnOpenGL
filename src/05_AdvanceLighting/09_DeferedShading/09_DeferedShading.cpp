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
#include <LOGL/Model.h>

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


	//------------ Shape
	Cube cube;


	//------------ 模型 . Cube
	vector<VAO::VBO_DataPatch> P3N3T2_Cube_Vec_VBO_Data_Patch = {
		{cube.GetVertexArr(), cube.GetVertexArrSize(), 3},
		{cube.GetNormalArr(), cube.GetNormalArrSize(), 3},
		{cube.GetTexCoordsArr(), cube.GetTexCoordsArrSize(), 2},
	};
	VAO VAO_P3N3T2_Cube(P3N3T2_Cube_Vec_VBO_Data_Patch, cube.GetIndexArr(), cube.GetIndexArrSize());


	//------------ 模型 . VAO_P3T2_Cube
	vector<VAO::VBO_DataPatch> VAO_P3T2_Cube_Vec_VBO_Data_Patch = {
		{cube.GetVertexArr(), cube.GetVertexArrSize(), 3},
		{cube.GetTexCoordsArr(), cube.GetTexCoordsArrSize(), 2},
	};
	VAO VAO_P3T2_Cube(VAO_P3T2_Cube_Vec_VBO_Data_Patch, cube.GetIndexArr(), cube.GetIndexArrSize());


	//------------ 模型 . VAO_P3_Cube
	VAO VAO_P3_Cube(cube.GetVertexArr(), cube.GetVertexArrSize(), { 3 }, cube.GetIndexArr(), cube.GetIndexArrSize());


	//------------ 模型 . Screen
	VAO VAO_Screen(&(data_ScreenVertices[0]), sizeof(data_ScreenVertices), { 2,2 });
	
	//------------ 模型 . Nanosuit
	Model nanosuit(rootPath + str_Obj_Nanosuit);

	//------------ 纹理
	const int textureNum = 2;
	Texture textures[textureNum];
	string imgPath[textureNum] = {
		rootPath + str_Img_Wood,
		rootPath + str_Img_Earth,
	};
	bool flip[textureNum] = { true, true };
	bool gammaCorrection[textureNum] = { true, true };
	for (size_t i = 0; i < textureNum; i++) {
		if (!textures[i].Load(imgPath[i], flip[i], gammaCorrection[i])) {
			printf("ERROR: Load texture [%s] fail.\n", imgPath[i].c_str());
			return 1;
		}
	}

	//------------ GBuffer Shader
	string gBuffer_vs = rootPath + str_GBuffer_vs;
	string gBuffer_fs = rootPath + str_GBuffer_fs;
	Shader gBufferShader(gBuffer_vs, gBuffer_fs);
	if (!gBufferShader.IsValid()) {
		printf("ERROR: gBufferShader load fail\n");
		return 1;
	}
	gBufferShader.UniformBlockBind("CameraMatrixs", 0);


	//------------ DeferedShading Shader
	string deferedShading_vs = rootPath + str_DeferedShading_vs;
	string deferedShading_fs = rootPath + str_DeferedShading_fs;
	Shader deferedShadingShader(deferedShading_vs, deferedShading_fs);
	if (!deferedShadingShader.IsValid()) {
		printf("ERROR: deferedShadingShader load fail\n");
		return 1;
	}
	deferedShadingShader.UniformBlockBind("CameraMatrixs", 0);
	deferedShadingShader.SetInt("gPosition", 0);
	deferedShadingShader.SetInt("gNormal", 1);
	deferedShadingShader.SetInt("gAlbedoSpec", 2);
	deferedShadingShader.SetInt("albedo", 3);
	const unsigned int NR_LIGHTS = 32;
	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColors;
	srand(13);
	for (unsigned int i = 0; i < NR_LIGHTS; i++)
	{
		// calculate slightly random offsets
		float xPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
		float yPos = ((rand() % 100) / 100.0) * 6.0 - 4.0;
		float zPos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
		lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
		// also calculate random color
		float rColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
		float gColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
		float bColor = ((rand() % 100) / 200.0f) + 0.5; // between 0.5 and 1.0
		lightColors.push_back(glm::vec3(rColor, gColor, bColor));
	}
	//const float constant = 1.0; // note that we don't send this to the shader, we assume it is always 1.0 (in our case)
	const float linear = 0.7;
	const float quadratic = 1.8;
	for (size_t i = 0; i < lightPositions.size(); i++)
	{
		deferedShadingShader.SetVec3f("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
		deferedShadingShader.SetVec3f("lights[" + std::to_string(i) + "].Color", lightColors[i]);
		// update attenuation parameters and calculate radius
		
		deferedShadingShader.SetFloat("lights[" + std::to_string(i) + "].Linear", linear);
		deferedShadingShader.SetFloat("lights[" + std::to_string(i) + "].Quadratic", quadratic);
	}

	//------------ LightBox Shader
	string lightBox_vs = rootPath + str_LightBox_vs;
	string lightBox_fs = rootPath + str_LightBox_fs;
	Shader lightBoxShader(lightBox_vs, lightBox_fs);
	if (!lightBoxShader.IsValid()) {
		printf("ERROR: lightBoxShader load fail\n");
		return 1;
	}
	lightBoxShader.UniformBlockBind("CameraMatrixs", 0);


	//------------ 相机
	float moveSpeed = *config->GetFloatPtr(config_CameraMoveSpeed);
	float rotateSpeed = *config->GetFloatPtr(config_CameraRotateSensitivity);
	Camera mainCamera(val_RatioWH, moveSpeed, rotateSpeed, glm::vec3(0.0f, 0.0f, 5.0f));
	GStorage<Camera *>::GetInstance()->Register(str_MainCamera.c_str(), &mainCamera);
	

	//------------ Camera Matrixs UBO
	size_t cameraMatrixsUBO;
	glGenBuffers(1, &cameraMatrixsUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, cameraMatrixsUBO);
	glBufferData(GL_UNIFORM_BUFFER, 2*sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, cameraMatrixsUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	

	//------------ GBuffer
	FBO FBO_GBuffer(val_windowWidth, val_windowHeight, FBO::ENUM_TYPE_GBUFFER);

	//------------ 操作

	// 注册输入

	auto registerInputOp = new RegisterInput(false);
	 

	// 更新

	float deltaTime = 0.0f; // 当前帧与上一帧的时间差
	GStorage<float *>::GetInstance()->Register(str_DeltaTime.c_str(), &deltaTime);
	float lastFrame = 0.0f; // 上一帧的时间
	auto timeUpdate = new LambdaOp([&]() {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	});

	auto cameraMatrixsUBO_Update = new LambdaOp([&]() {
		glBindBuffer(GL_UNIFORM_BUFFER, cameraMatrixsUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(mainCamera.GetViewMatrix()));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(mainCamera.GetProjectionMatrix()));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	});

	auto updateOpQueue = new OpQueue;
	(*updateOpQueue) << timeUpdate << cameraMatrixsUBO_Update;

	auto nanosuitOp = Operation::ToPtr(new LambdaOp([&]() { 
		for (size_t i = 0; i < val_NanosuitNum; i++) 
		{
			glm::mat4 model(1.0f);
			model = glm::translate(model, data_NanosuitPositions[i]);
			model = glm::scale(model, glm::vec3(0.25f));
			gBufferShader.SetMat4f("model", model);
			nanosuit.Draw(gBufferShader);
		}
	}));

	auto gBufferOp = new OpNode([&]() {
		FBO_GBuffer.Use();
		glClearColor(0, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	});
	(*gBufferOp) << nanosuitOp;
	
	auto deferedShadingOp = Operation::ToPtr(new LambdaOp([&]() {
		deferedShadingShader.SetVec3f("viewPos", mainCamera.GetPos());
		FBO_GBuffer.GetColorTexture(0).Use(0);// position
		FBO_GBuffer.GetColorTexture(1).Use(1);// normal
		FBO_GBuffer.GetColorTexture(2).Use(2);// albedo_spec
		textures[1].Use(3);
		VAO_Screen.Draw();
	}));

	auto lightBoxOp = Operation::ToPtr(new LambdaOp([&]() {
		FBO_GBuffer.PassTo(FBO::DefaultBuffer, val_windowWidth, val_windowHeight, FBO::ENUM_PASS_TYPE::ENUM_PASS_DEPTH);
		for (size_t i = 0; i < lightPositions.size(); i++) {
			glm::mat4 model(1.0f);
			model = glm::translate(model, lightPositions[i]);
			model = glm::scale(model, glm::vec3(0.25f));
			lightBoxShader.SetMat4f("model", model);
			lightBoxShader.SetVec3f("color", lightColors[i]);
			VAO_P3_Cube.Draw();
		}
	}));
	 
	auto defaultBufferOp = new OpNode([]() {//init
		FBO::UseDefault();
		glEnable(GL_DEPTH_TEST);
		glClearColor(0, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}, []() {//end
		glfwSwapBuffers(Glfw::GetInstance()->GetWindow());
		glfwPollEvents();
	});

	(*defaultBufferOp) << deferedShadingOp << lightBoxOp;

	auto renderQueue = new OpQueue;
	(*renderQueue) << gBufferOp << defaultBufferOp;
	
	// 帧操作

	auto opQueue = new OpQueue;
	(*opQueue) << registerInputOp << updateOpQueue << renderQueue;
	
	//------------
	Glfw::GetInstance()->Run(opQueue);
	
	//------------
	Glfw::GetInstance()->Terminate();
	
	return 0;
}

