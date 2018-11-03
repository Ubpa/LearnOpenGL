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
#include <random>

#include "Defines.h"
#include "RegisterInput.h"
 
using namespace LOGL;
using namespace std; 
using namespace Ubpa;
using namespace Define;

float lerp(float a, float b, float f){ return a + f * (b - a); }

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


	//------------ 相机
	float moveSpeed = *config->GetFloatPtr(config_CameraMoveSpeed);
	float rotateSpeed = *config->GetFloatPtr(config_CameraRotateSensitivity);
	Camera mainCamera(val_RatioWH, moveSpeed, rotateSpeed, glm::vec3(0.0f, 0.0f, 5.0f));
	GStorage<Camera *>::GetInstance()->Register(str_MainCamera.c_str(), &mainCamera);


	//------------ GBuffer Shader
	string gBuffer_vs = rootPath + str_GBuffer_vs;
	string gBuffer_fs = rootPath + str_GBuffer_fs;
	Shader gBufferShader(gBuffer_vs, gBuffer_fs);
	if (!gBufferShader.IsValid()) {
		printf("ERROR: gBufferShader load fail\n");
		return 1;
	}
	gBufferShader.UniformBlockBind("CameraMatrixs", 0);


	//------------ SSAO Shader
	string SSAO_vs = rootPath + str_SSAO_vs;
	string SSAO_fs = rootPath + str_SSAO_fs;
	Shader SSAOShader(SSAO_vs, SSAO_fs);
	if (!SSAOShader.IsValid()) {
		printf("ERROR: SSAOShader load fail\n");
		return 1;
	}
	SSAOShader.SetInt("gPosition", 0);
	SSAOShader.SetInt("gNormal", 1);
	SSAOShader.SetInt("texNoise", 2);
	bool ssaoEnable = true;
	GStorage<bool *>::GetInstance()->Register("bool_ptr_ssaoEnable", &ssaoEnable);
	// generate sample kernel
	// ----------------------
	uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
	default_random_engine generator;
	vector<glm::vec3> ssaoKernel;
	for (size_t i = 0; i < 64; ++i){
		glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		float scale = float(i) / 64.0;

		// scale samples s.t. they're more aligned to center of kernel
		scale = lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		ssaoKernel.push_back(sample);
	}
	for (size_t i = 0; i < 64; ++i)
		SSAOShader.SetVec3f("samples[" + std::to_string(i) + "]", ssaoKernel[i]);

	// generate noise texture
	// ----------------------
	vector<glm::vec3> ssaoNoise;
	for (size_t i = 0; i < 16; i++){
		glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
		ssaoNoise.push_back(noise);
	}
	Texture noiseTexture(4, 4, &(ssaoNoise[0][0]), GL_FLOAT, GL_RGB, GL_RGB32F);


	//------------ Blur Shader
	string blur_vs = rootPath + str_Blur_vs;
	string blur_fs = rootPath + str_Blur_fs;
	Shader blurShader(blur_vs, blur_fs);
	if (!blurShader.IsValid()) {
		printf("ERROR: blurShader load fail\n");
		return 1;
	}
	blurShader.SetInt("SSAO", 0);


	//------------ DeferedShading Shader
	string deferedShading_vs = rootPath + str_DeferedShading_vs;
	string deferedShading_fs = rootPath + str_DeferedShading_fs;
	Shader deferedShadingShader(deferedShading_vs, deferedShading_fs);
	if (!deferedShadingShader.IsValid()) {
		printf("ERROR: deferedShadingShader load fail\n");
		return 1;
	}
	deferedShadingShader.SetInt("gPosition", 0);
	deferedShadingShader.SetInt("gNormal", 1);
	deferedShadingShader.SetInt("gAlbedoSpec", 2);
	deferedShadingShader.SetInt("SSAO", 3);
	const glm::vec3 lightPos = glm::vec3(2.0, 4.0, -2.0);
	const glm::vec3 lightColor = glm::vec3(0.2, 0.2, 0.7);
	deferedShadingShader.SetVec3f("light.Color", lightColor);
	// Update attenuation parameters
	const float constant = 1.0; // note that we don't send this to the shader, we assume it is always 1.0 (in our case)
	const float linear = 0.09;
	const float quadratic = 0.032;
	deferedShadingShader.SetFloat("light.Linear", linear);
	deferedShadingShader.SetFloat("light.Quadratic", quadratic);

	

	//------------ Camera Matrixs UBO
	size_t cameraMatrixsUBO;
	glGenBuffers(1, &cameraMatrixsUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, cameraMatrixsUBO);
	glBufferData(GL_UNIFORM_BUFFER, 2*sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, cameraMatrixsUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	

	//------------ GBuffer
	FBO FBO_GBuffer(val_windowWidth, val_windowHeight, FBO::ENUM_TYPE_GBUFFER);


	//------------ SSAO FBO
	FBO FBO_SSAO(val_windowWidth, val_windowHeight, FBO::ENUM_TYPE_RED);


	//------------ Blur FBO
	FBO FBO_Blur(val_windowWidth, val_windowHeight, FBO::ENUM_TYPE_RED);


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
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
		model = glm::scale(model, glm::vec3(0.5f));
		gBufferShader.SetMat4f("model", model);
		gBufferShader.SetBool("invertedNormals", false);
		nanosuit.Draw(gBufferShader);
	})); 

	auto roomOp = Operation::ToPtr(new LambdaOp([&]() {
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0, 7.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f, 15.0f, 15.0f));
		gBufferShader.SetMat4f("model", model);
		gBufferShader.SetBool("invertedNormals", true); // invert normals as we're inside the cube
		VAO_P3N3T2_Cube.Draw();
	}));

	auto gBufferOp = new OpNode([&]() {
		FBO_GBuffer.Use();
		glClearColor(0, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	});
	(*gBufferOp) << roomOp << nanosuitOp;
	
	auto ssaoOp = Operation::ToPtr(new LambdaOp([&]() {
		FBO_SSAO.Use();
		glClearColor(0, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		SSAOShader.SetMat4f("projection", mainCamera.GetProjectionMatrix());
		FBO_GBuffer.GetColorTexture(0).Use(0);// position
		FBO_GBuffer.GetColorTexture(1).Use(1);// normal
		noiseTexture.Use(2);
		VAO_Screen.Draw();
	}));

	auto blurOp = Operation::ToPtr(new LambdaOp([&]() {
		FBO_Blur.Use();
		glClearColor(0, 0, 0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		FBO_SSAO.GetColorTexture().Use();
		blurShader.Use();
		VAO_Screen.Draw();
	}));

	auto geometryQueue = new OpQueue;
	(*geometryQueue) << gBufferOp << ssaoOp << blurOp;

	auto deferedShadingOp = Operation::ToPtr(new LambdaOp([&]() {
		deferedShadingShader.SetVec3f("viewPos", mainCamera.GetPos());
		// send light relevant uniforms
		glm::vec3 lightPosView = glm::vec3(mainCamera.GetViewMatrix() * glm::vec4(lightPos, 1.0));
		deferedShadingShader.SetVec3f("light.Position", lightPosView);
		deferedShadingShader.SetBool("ssaoEnable", ssaoEnable);

		FBO_GBuffer.GetColorTexture(0).Use(0);// position
		FBO_GBuffer.GetColorTexture(1).Use(1);// normal
		FBO_GBuffer.GetColorTexture(2).Use(2);// albedo_spec
		FBO_Blur.GetColorTexture().Use(3);

		VAO_Screen.Draw();
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

	(*defaultBufferOp) << deferedShadingOp;

	auto renderQueue = new OpQueue;
	(*renderQueue) << geometryQueue << defaultBufferOp;
	
	// 帧操作

	auto opQueue = new OpQueue;
	(*opQueue) << registerInputOp << updateOpQueue << renderQueue;
	
	//------------
	Glfw::GetInstance()->Run(opQueue);
	
	//------------
	Glfw::GetInstance()->Terminate();
	
	return 0;
}

