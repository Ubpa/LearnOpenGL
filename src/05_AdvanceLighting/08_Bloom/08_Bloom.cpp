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


	//------------ Shape
	Cube cube;


	//------------ 模型 . P3N3T2_Cube
	vector<VAO::VBO_DataPatch> P3N3T2_Cube_Vec_VBO_Data_Patch = {
		{cube.GetVertexArr(), cube.GetVertexArrSize(), 3},
		{cube.GetNormalArr(), cube.GetNormalArrSize(), 3},
		{cube.GetTexCoordsArr(), cube.GetTexCoordsArrSize(), 2},
	};
	VAO VAO_P3N3T2_Cube(P3N3T2_Cube_Vec_VBO_Data_Patch, cube.GetIndexArr(), cube.GetIndexArrSize());


	//------------ 模型 . P3T2_Cube
	vector<VAO::VBO_DataPatch> P3T2_Cube_Vec_VBO_Data_Patch = {
		{cube.GetVertexArr(), cube.GetVertexArrSize(), 3},
		{cube.GetTexCoordsArr(), cube.GetTexCoordsArrSize(), 2},
	};
	VAO VAO_P3T2_Cube(P3T2_Cube_Vec_VBO_Data_Patch, cube.GetIndexArr(), cube.GetIndexArrSize());


	//------------ 模型 . Screen
	VAO VAO_Screen(&(data_ScreenVertices[0]), sizeof(data_ScreenVertices), { 2,2 });
	

	//------------ BloomObj Shader
	string bloomObj_vs = rootPath + str_BloomObj_vs;
	string bloomObj_fs = rootPath + str_BloomObj_fs;
	Shader bloomObjShader(bloomObj_vs, bloomObj_fs);
	if (!bloomObjShader.IsValid()) {
		printf("ERROR: bloomObjShader load fail\n");
		return 1;
	}
	bloomObjShader.UniformBlockBind("CameraMatrixs", 0);
	bloomObjShader.SetInt("diffuseTexture", 0);
	const size_t lightNum = 4;
	for (size_t i = 0; i < lightNum; i++) {
		bloomObjShader.SetVec3f("lights[" + to_string(i) + "].Position", data_LightPositions[i]);
		bloomObjShader.SetVec3f("lights[" + to_string(i) + "].Color", data_LightColors[i]);
	}

	//------------ BloomLight Shader
	string bloomLight_vs = rootPath + str_BloomLight_vs;
	string bloomLight_fs = rootPath + str_BloomLight_fs;
	Shader bloomLightShader(bloomLight_vs, bloomLight_fs);
	if (!bloomLightShader.IsValid()) {
		printf("ERROR: bloomLightShader load fail\n");
		return 1;
	}
	bloomLightShader.UniformBlockBind("CameraMatrixs", 0);


	//------------ Blur Shader
	string blur_vs = rootPath + str_Blur_vs;
	string blur_fs = rootPath + str_Blur_fs;
	Shader blurShader(blur_vs, blur_fs);
	if (!blurShader.IsValid()) {
		printf("ERROR: blurShader load fail\n");
		return 1;
	}
	blurShader.SetInt("image", 0);

	//------------ Blend Shader
	string blend_vs = rootPath + str_Blend_vs;
	string blend_fs = rootPath + str_Blend_fs;
	Shader blendShader(blend_vs, blend_fs);
	if (!blendShader.IsValid()) {
		printf("ERROR: blendShader load fail\n");
		return 1;
	}
	blendShader.SetInt("scene", 0);
	blendShader.SetInt("bloomBlur", 1);
	float exposure = 1.0f;
	GStorage<float *>::GetInstance()->Register("float_ptr_exposure", &exposure);
	bool bloomEnable = true;
	GStorage<bool*>::GetInstance()->Register("bool_ptr_bloomEnable", &bloomEnable);

	//------------ 纹理
	const int textureNum = 1;
	Texture textures[textureNum];
	string imgPath[textureNum] = {
		rootPath + str_Img_Container2,
	};
	bool flip[textureNum] = { true };
	bool gammaCorrection[textureNum] = { true };
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
	
	//------------ Camera Matrixs UBO
	size_t cameraMatrixsUBO;
	glGenBuffers(1, &cameraMatrixsUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, cameraMatrixsUBO);
	glBufferData(GL_UNIFORM_BUFFER, 2*sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, cameraMatrixsUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);


	//------------ HDR_Bloom 帧缓冲
	FBO FBO_HDR_Bloom(val_windowWidth, val_windowHeight, FBO::ENUM_TYPE_RGBF2_DEPTH);

	
	//------------ Blur 帧缓冲
	FBO blurFBOs[2] = {
		FBO(val_windowWidth, val_windowHeight, FBO::ENUM_TYPE_COLOR_FLOAT),
		FBO(val_windowWidth, val_windowHeight, FBO::ENUM_TYPE_COLOR_FLOAT)
	};


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
	auto objOp = Operation::ToPtr(new LambdaOp([&]() {
		textures[0].Use();
		bloomObjShader.SetVec3f("viewPos", mainCamera.GetPos());

		glm::mat4 model;
		// create one large cube that acts as the floor
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0));
		model = glm::scale(model, glm::vec3(25.0f, 1.0f, 25.0f));
		bloomObjShader.SetMat4f("model", model);
		VAO_P3N3T2_Cube.Draw();

		// then create multiple cubes as the scenery
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
		bloomObjShader.SetMat4f("model", model);
		VAO_P3N3T2_Cube.Draw();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
		bloomObjShader.SetMat4f("model", model);
		VAO_P3N3T2_Cube.Draw();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 2.0));
		model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		model = glm::scale(model, glm::vec3(2.0f));
		bloomObjShader.SetMat4f("model", model);
		VAO_P3N3T2_Cube.Draw();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 2.7f, 4.0));
		model = glm::rotate(model, glm::radians(23.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		model = glm::scale(model, glm::vec3(2.5f));
		bloomObjShader.SetMat4f("model", model);
		VAO_P3N3T2_Cube.Draw();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -3.0));
		model = glm::rotate(model, glm::radians(124.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		model = glm::scale(model, glm::vec3(2.0f));
		bloomObjShader.SetMat4f("model", model);
		VAO_P3N3T2_Cube.Draw();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0));
		bloomObjShader.SetMat4f("model", model);
		VAO_P3N3T2_Cube.Draw();
	}));

	auto lightOp = Operation::ToPtr(new LambdaOp([&]() {
		for (size_t i = 0; i < lightNum; i++) {
			glm::mat4 model(1.0f);
			model = glm::translate(model, data_LightPositions[i]);
			model = glm::scale(model, glm::vec3(0.5f));
			bloomLightShader.SetMat4f("model", model);
			bloomLightShader.SetVec3f("lightColor", data_LightColors[i]);
			VAO_P3T2_Cube.Draw();
		}
	}));

	auto bloomOp = new OpNode([&]() {
		FBO_HDR_Bloom.Use();
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}, []() {
		FBO::UseDefault(); 
	});
	(*bloomOp) << objOp << lightOp;
	
	auto blurOp = Operation::ToPtr(new LambdaOp([&]() {
		if (!bloomEnable)
			return;

		bool horizontal = true;
		bool first_iteration = true;
		for (size_t i = 0; i < 10; i++) {
			blurFBOs[horizontal].Use();
			blurShader.SetBool("horizontal", horizontal);
			if (first_iteration)
				FBO_HDR_Bloom.GetColorTexture(1).Use();
			else
				blurFBOs[!horizontal].GetColorTexture().Use();

			VAO_Screen.Draw();
			horizontal = !horizontal;
			first_iteration = false;
		}
		FBO::UseDefault();
	}));

	auto blendOp = Operation::ToPtr(new LambdaOp([&](){
		FBO_HDR_Bloom.GetColorTexture(0).Use(0);
		blurFBOs[0].GetColorTexture().Use(1);
		blendShader.SetFloat("exposure", exposure);
		blendShader.SetBool("bloomEnable", bloomEnable);
		VAO_Screen.Draw();
	}));

	auto renderOp = new OpNode([]() {//init
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}, []() {//end
		glfwSwapBuffers(Glfw::GetInstance()->GetWindow());
		glfwPollEvents();
	});

	(*renderOp) << bloomOp << blurOp << blendOp;
	
	//------------- 整合
	auto opQueue = new OpQueue;
	(*opQueue) << registerInputOp << updateOpQueue << renderOp;
	
	//------------
	Glfw::GetInstance()->Run(opQueue);
	
	//------------
	Glfw::GetInstance()->Terminate();
	
	return 0;
}

