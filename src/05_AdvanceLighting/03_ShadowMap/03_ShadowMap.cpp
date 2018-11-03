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
	
	
	//------------ ����
	float ratioWH = (float)val_windowWidth / (float)val_windowHeight;
	string windowTitle = str_Chapter + "/" + str_Subchapter;
	Glfw::GetInstance()->Init(val_windowWidth, val_windowHeight, windowTitle.c_str());
	Glfw::GetInstance()->LockCursor();

	glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);

	//------------ ģ�� . ƽ��
	VAO VAO_Panel(&(data_PlaneVertices[0]), sizeof(data_PlaneVertices), { 3,3,2 });


	//------------ ģ�� . Cube
	Cube cube;
	vector<VAO::VBO_DataPatch> cube_Vec_VBO_Data_Patch = {
		{cube.GetVertexArr(), cube.GetVertexArrSize(), 3},
		{cube.GetNormalArr(), cube.GetNormalArrSize(), 3},
		{cube.GetTexCoordsArr(), cube.GetTexCoordsArrSize(), 2},
	};
	VAO VAO_Cube(cube_Vec_VBO_Data_Patch, cube.GetIndexArr(), cube.GetIndexArrSize());


	//------------ Depth ��ɫ��
	string depth_vs = rootPath + str_Depth_vs;
	string depth_fs = rootPath + str_Depth_fs;
	Shader depthShader(depth_vs, depth_fs);
	if (!depthShader.IsValid()) {
		printf("depthShader load fail\n");
		return 1;
	}
	depthShader.UniformBlockBind("LightMatrix", 1);

	//------------ Shadow ��ɫ��
	string shadow_vs = rootPath + str_Shadow_vs;
	string shadow_fs = rootPath + str_Shadow_fs;
	Shader shadowShader(shadow_vs, shadow_fs);
	if (!shadowShader.IsValid()) {
		printf("shadowShader load fail\n");
		return 1;
	}
	shadowShader.SetInt("diffuseTexture", 0);
	shadowShader.SetInt("shadowMap", 1);
	shadowShader.UniformBlockBind("CameraMatrixs", 0);
	shadowShader.UniformBlockBind("LightMatrix", 1);
	shadowShader.SetVec3f("lightPos", lightPos);

	//------------ ����
	const int textureNum = 1;
	Texture textures[textureNum];
	string imgPath[textureNum] = {
		rootPath + str_Img_Wood
	}; 
	bool flip[textureNum] = { false };
	for (size_t i = 0; i < textureNum; i++) {
		if (!textures[i].Load(imgPath[i], flip[i])) {
			printf("Load texture [%s] fail.\n", imgPath[i].c_str());
			return 1;
		}
	}

	//------------ ���
	float moveSpeed = *config->GetFloatPtr(config_CameraMoveSpeed);
	float rotateSpeed = *config->GetFloatPtr(config_CameraRotateSensitivity);
	Camera mainCamera(ratioWH, moveSpeed, rotateSpeed, glm::vec3(0.0f, 0.0f, 3.0f));
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

	
	//------------ ��Ȼ���
	FBO FBO_DepthMap(1024, 1024, FBO::ENUM_TYPE_DEPTH);
	Texture depthMap(FBO_DepthMap.GetDepthTexture().GetID());


	//------------ ����
	auto registerInputOp = new RegisterInput(false);
	 
	//------------- ʱ��
	float deltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
	GStorage<float *>::GetInstance()->Register(str_DeltaTime.c_str(), &deltaTime);
	float lastFrame = 0.0f; // ��һ֡��ʱ��
	auto timeOp = new LambdaOp([&]() {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	});

	//------------ �������
	auto cameraMatrixsUBO_Update = new LambdaOp([&]() {
		glBindBuffer(GL_UNIFORM_BUFFER, cameraMatrixsUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(mainCamera.GetViewMatrix()));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(mainCamera.GetProjectionMatrix()));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	});

	//------------ ģ�ͳ���
	Shader * curShader = NULL;
	auto sceneOp = Operation::ToPtr(new LambdaOp([&]() {
		// floor
		curShader->SetMat4f("model", glm::mat4(1.0f));
		VAO_Panel.Use();
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glm::mat4 model;
		// cubes
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f));
		curShader->SetMat4f("model", model);
		VAO_Cube.Use();
		glDrawElements(GL_TRIANGLES, cube.GetTriNum() * 3, GL_UNSIGNED_INT, NULL);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(0.5f));
		curShader->SetMat4f("model", model);
		VAO_Cube.Use();
		glDrawElements(GL_TRIANGLES, cube.GetTriNum() * 3, GL_UNSIGNED_INT, NULL);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
		model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		model = glm::scale(model, glm::vec3(0.25));
		curShader->SetMat4f("model", model);
		VAO_Cube.Use();
		glDrawElements(GL_TRIANGLES, cube.GetTriNum() * 3, GL_UNSIGNED_INT, NULL);
	}));

	//------------ ��Ⱦ���ͼ
	auto depthOp = new OpNode([&]() {//init
		glEnable(GL_DEPTH_TEST);
		glViewport(0, 0, 1024, 1024);
		FBO_DepthMap.Use();
		glClear(GL_DEPTH_BUFFER_BIT);
		textures[0].Use();
		curShader = &depthShader;
	},
		[]() {//end
		FBO::UseDefault();
		glViewport(0, 0, val_windowWidth, val_windowHeight);
	});
	(*depthOp) << sceneOp;

	//------------ ��Ⱦ����Ӱ�ĳ���
	auto shadowOp = new OpNode([&]() {//init
		curShader = &shadowShader;
		curShader->SetVec3f("viewPos", mainCamera.GetPos());
		textures[0].Use(0);
		depthMap.Use(1);
	});
	(*shadowOp) << sceneOp;

	//------------ ��Ⱦ����
	auto renderOp = new OpNode([]() {//init
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}, []() {//end
		glfwSwapBuffers(Glfw::GetInstance()->GetWindow());
		glfwPollEvents();
	});

	(*renderOp) << shadowOp;
	
	//------------- ����
	auto opQueue = new OpQueue;
	(*opQueue) << registerInputOp << timeOp << cameraMatrixsUBO_Update << depthOp << renderOp;
	
	//------------
	Glfw::GetInstance()->Run(opQueue);
	
	//------------
	Glfw::GetInstance()->Terminate();
	
	return 0;
}

