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

	glm::vec3 lightPos(0, 0, 0);


	//------------ 模型 . Cube
	Cube cube;
	vector<VAO::VBO_DataPatch> cube_Vec_VBO_Data_Patch = {
		{cube.GetVertexArr(), cube.GetVertexArrSize(), 3},
		{cube.GetNormalArr(), cube.GetNormalArrSize(), 3},
		{cube.GetTexCoordsArr(), cube.GetTexCoordsArrSize(), 2},
	};
	VAO VAO_Cube(cube_Vec_VBO_Data_Patch, cube.GetIndexArr(), cube.GetIndexArrSize());


	//------------ Depth 着色器
	string depth_vs = rootPath + str_Depth_vs;
	string depth_fs = rootPath + str_Depth_fs;
	string depth_gs = rootPath + str_Depth_gs;
	Shader depthShader(depth_vs, depth_fs, depth_gs);
	if (!depthShader.IsValid()) {
		printf("ERROR: depthShader load fail\n");
		return 1;
	}
	depthShader.SetFloat("far_plane", 25.0f);

	//------------ Shadow 着色器
	string shadow_vs = rootPath + str_Shadow_vs;
	string shadow_fs = rootPath + str_Shadow_fs;
	Shader shadowShader(shadow_vs, shadow_fs);
	if (!shadowShader.IsValid()) {
		printf("ERROR: shadowShader load fail\n");
		return 1;
	}
	shadowShader.SetInt("diffuseTexture", 0);
	shadowShader.SetInt("depthMap", 1);
	shadowShader.UniformBlockBind("CameraMatrixs", 0);
	shadowShader.SetBool("shadowEnable", true);
	shadowShader.SetFloat("far_plane", 25.0f);

	//------------ 纹理
	const int textureNum = 1;
	Texture textures[textureNum];
	string imgPath[textureNum] = {
		rootPath + str_Img_Wood
	}; 
	bool flip[textureNum] = { false };
	for (size_t i = 0; i < textureNum; i++) {
		if (!textures[i].Load(imgPath[i], flip[i])) {
			printf("ERROR: Load texture [%s] fail.\n", imgPath[i].c_str());
			return 1;
		}
	}

	//------------ 相机
	float moveSpeed = *config->GetFloatPtr(config_CameraMoveSpeed);
	float rotateSpeed = *config->GetFloatPtr(config_CameraRotateSensitivity);
	Camera mainCamera(val_RatioWH, moveSpeed, rotateSpeed, glm::vec3(0.0f, 0.0f, 3.0f));
	GStorage<Camera *>::GetInstance()->Register(str_MainCamera.c_str(), &mainCamera);
	
	//------------ Camera Matrixs UBO
	size_t cameraMatrixsUBO;
	glGenBuffers(1, &cameraMatrixsUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, cameraMatrixsUBO);
	glBufferData(GL_UNIFORM_BUFFER, 2*sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, cameraMatrixsUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//------------ Light Matrix UBO
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 1.0f, far_plane = 7.5f;
	lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;

	size_t lightMatrixUBO;
	glGenBuffers(1, &lightMatrixUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, lightMatrixUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), glm::value_ptr(lightSpaceMatrix), GL_STATIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, lightMatrixUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	
	//------------ 深度缓冲
	FBO FBO_DepthMap(val_ShadowWidth, val_ShadowHeight, FBO::ENUM_TYPE_CUBE_DEPTH);
	Texture depthMap(FBO_DepthMap.GetDepthTexture().GetID(), Texture::ENUM_TYPE_CUBE_MAP);


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

	//------------ 更新灯光
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), val_ShadowRatioWH, 1.0f, 25.0f);
	auto lightPosUpdate = new LambdaOp([&]() {
		lightPos.z = sin(glfwGetTime() * 0.5f) * 3.0f;
		shadowShader.SetVec3f("lightPos", lightPos);
		//------------ 深度相机
		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj *
			glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

		for (size_t i = 0; i < 6; ++i)
			depthShader.SetMat4f("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);

		depthShader.SetVec3f("lightPos", lightPos);
	});

	auto updateOpQueue = new OpQueue;
	(*updateOpQueue) << timeUpdate << cameraMatrixsUBO_Update << lightPosUpdate;

	//------------ 模型场景
	Shader * curShader = NULL;
	auto sceneOp = Operation::ToPtr(new LambdaOp([&]() {
		// room cube
		glm::mat4 model;
		model = glm::scale(model, glm::vec3(10.0f));
		curShader->SetMat4f("model", model);
		glDisable(GL_CULL_FACE); // note that we disable culling here since we render 'inside' the cube instead of the usual 'outside' which throws off the normal culling methods.
		curShader->SetBool("reverse_normals", true); // A small little hack to invert normals when drawing cube from the inside so lighting still works.
		VAO_Cube.Use();
		glDrawElements(GL_TRIANGLES, cube.GetTriNum() * 3, GL_UNSIGNED_INT, NULL);
		curShader->SetBool("reverse_normals", false); // and of course disable it
		glEnable(GL_CULL_FACE);

		// cubes
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.0f, -3.5f, 0.0));
		model = glm::scale(model, glm::vec3(1.0f));
		curShader->SetMat4f("model", model);
		VAO_Cube.Use();
		glDrawElements(GL_TRIANGLES, cube.GetTriNum() * 3, GL_UNSIGNED_INT, NULL);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 3.0f, 1.0));
		model = glm::scale(model, glm::vec3(1.5f));
		curShader->SetMat4f("model", model);
		VAO_Cube.Use();
		glDrawElements(GL_TRIANGLES, cube.GetTriNum() * 3, GL_UNSIGNED_INT, NULL);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 1.5));
		model = glm::scale(model, glm::vec3(1.0f));
		curShader->SetMat4f("model", model);
		VAO_Cube.Use();
		glDrawElements(GL_TRIANGLES, cube.GetTriNum() * 3, GL_UNSIGNED_INT, NULL);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.5f, 2.0f, -3.0));
		model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		model = glm::scale(model, glm::vec3(1.5f)); 
		curShader->SetMat4f("model", model);
		VAO_Cube.Use();
		glDrawElements(GL_TRIANGLES, cube.GetTriNum() * 3, GL_UNSIGNED_INT, NULL);
	}));

	//------------ 渲染深度图
	auto depthOp = new OpNode([&]() {//init
		glEnable(GL_DEPTH_TEST);
		glViewport(0, 0, val_ShadowWidth, val_ShadowHeight);
		FBO_DepthMap.Use();
		glClear(GL_DEPTH_BUFFER_BIT);
		curShader = &depthShader;
	},
		[]() {//end
		FBO::UseDefault();
		glViewport(0, 0, val_windowWidth, val_windowHeight);
	});
	(*depthOp) << sceneOp;

	//------------ 渲染带阴影的场景
	auto shadowOp = new OpNode([&]() {//init
		curShader = &shadowShader;
		curShader->SetVec3f("viewPos", mainCamera.GetPos());
		textures[0].Use(0);
		depthMap.Use(1);
	});
	(*shadowOp) << sceneOp;

	//------------ 渲染操作
	auto renderOp = new OpNode([]() {//init
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}, []() {//end
		glfwSwapBuffers(Glfw::GetInstance()->GetWindow());
		glfwPollEvents();
	});

	(*renderOp) << shadowOp;
	
	//------------- 整合
	auto opQueue = new OpQueue;
	(*opQueue) << registerInputOp << updateOpQueue << depthOp << renderOp;
	
	//------------
	Glfw::GetInstance()->Run(opQueue);
	
	//------------
	Glfw::GetInstance()->Terminate();
	
	return 0;
}

