#include <ROOT_PATH.h>

#include <Utility/Image.h>
#include <Utility/GStorage.h>
#include <Utility/LambdaOp.h>
#include <Utility/Config.h>
#include <Utility/Sphere.h>

#include <LOGL/Shader.h>
#include <LOGL/Camera.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Defines.h"
#include "RegisterInput.h"

using namespace LOGL;
using namespace std;
using namespace Ubpa;
using namespace Define;
//------------

Config * DoConfig();

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
	//------------
	Sphere sphere = Sphere(30);
	//------------ ������
	size_t VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	size_t VBO;
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeData), cubeData, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//------------ ��
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
	glBufferData(GL_ARRAY_BUFFER, sphere.GetVertexArrSize(), sphere.GetVertexArr(), GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(1);

	size_t sphereEBO;
	glGenBuffers(1, &sphereEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.GetIndexArrSize(), sphere.GetIndexArr(), GL_STATIC_DRAW);

	//------------ ��
	size_t lightVBO;
	glGenBuffers(1, &lightVBO);
	size_t lightVAO;
	glGenVertexArrays(1, &lightVAO);

	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(data_vertices), data_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	//------------ ��Դģ��
	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	//------------ ��Դ��ɫ��
	string light_vs = rootPath + str_Light_vs;
	string light_fs = rootPath + str_Light_fs;
	Shader lightShader(light_vs.c_str(), light_fs.c_str());
	if (!lightShader.IsValid()) {
		cout << "lightShader is not Valid\n";
		return 1;
	}
	//------------ ���
	Camera mainCamera(ratioWH, 0.1f, 100.0f, glm::vec3(0.0f, 0.0f, 4.0f));
	GStorage<Camera *>::GetInstance()->Register(str_MainCamera.c_str(), &mainCamera);
	//------------ ������ɫ��
	string lighting_vs = rootPath + str_Lighting_vs;
	string lighting_fs = rootPath + str_Lighting_fs;
	Shader lightingShader(lighting_vs.c_str(), lighting_fs.c_str());
	if (!lightingShader.IsValid()) {
		cout << "lightingShader is not Valid\n";
		return 1;
	}
	lightingShader.Use(); 
	//����
	lightingShader.SetVec3f("material.ambient", 1.0f, 0.5f, 0.31f);
	lightingShader.SetVec3f("material.diffuse", 1.0f, 0.5f, 0.31f);
	lightingShader.SetVec3f("material.specular", 0.5f, 0.5f, 0.5f);
	lightingShader.SetFloat("material.shininess", 32.0f);
	//��Դ
	lightingShader.SetVec3f("light.position", lightPos);
	lightingShader.SetVec3f("light.ambient", 0.2f, 0.2f, 0.2f);
	lightingShader.SetVec3f("light.diffuse", 0.8f, 0.8f, 0.8f); // �����յ�����һЩ�Դ��䳡��
	lightingShader.SetVec3f("light.specular", 1.0f, 1.0f, 1.0f);
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
	//------------ ���
	auto clearOp = new LambdaOp([]() {
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	});
	//------------ ��Ⱦ������ģ��
	auto cubeOp = new LambdaOp([&]() {
		lightingShader.Use();
		lightingShader.SetVec3f("viewPos", mainCamera.GetPos());
		glBindVertexArray(VAO);
		lightingShader.SetMat4f("view", mainCamera.GetViewMatrix());
		lightingShader.SetMat4f("projection", mainCamera.GetProjectionMatrix());
		glm::mat4 trans(1.0f);
		float t = glfwGetTime();
		trans = glm::scale(trans, glm::vec3(1.0f, 1.0f, 1.0f)*(sinf(t)*0.5f) + 1.0f);
		trans = glm::rotate(trans, t, glm::vec3(1.0f, 1.0f, 1.0f));
		lightingShader.SetMat4f("model", trans);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}); 

	//------------ ��Ⱦ����ģ��
	auto sphereOp = new LambdaOp([&]() {
		lightingShader.Use();
		lightingShader.SetVec3f("viewPos", mainCamera.GetPos());
		glm::vec3 lightColor;
		lightColor.x = 0.5f + 0.5f*sinf(glfwGetTime() * 2.0f);
		lightColor.y = 0.5f + 0.5f*sinf(glfwGetTime() * 0.7f);
		lightColor.z = 0.5f + 0.5f*sinf(glfwGetTime() * 1.3f);
		GStorage<glm::vec3>::GetInstance()->Register(str_LightColor, lightColor);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // ����Ӱ��
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // �ܵ͵�Ӱ��

		lightingShader.SetVec3f("light.ambient", ambientColor);
		lightingShader.SetVec3f("light.diffuse", diffuseColor);

		glBindVertexArray(sphereVAO);
		lightingShader.SetMat4f("view", mainCamera.GetViewMatrix());
		lightingShader.SetMat4f("projection", mainCamera.GetProjectionMatrix());
		glm::mat4 model = glm::mat4(1.0f);
		float t = glfwGetTime();
		model = glm::translate(model, glm::vec3(1.5f, -0.5f + sinf(2*t), -1.5f));
		lightingShader.SetMat4f("model", model);
		glDrawElements(GL_TRIANGLES, sphere.GetTriNum() * 3, GL_UNSIGNED_INT, NULL);
	});

	auto lightOp = new LambdaOp([&]() {
		lightShader.Use();
		glm::vec3 lightColor = *GStorage<glm::vec3>::GetInstance()->GetPtr(str_LightColor);
		lightShader.SetVec3f(str_LightColor, lightColor);
		glBindVertexArray(lightVAO);
		lightShader.SetMat4f("view", mainCamera.GetViewMatrix());
		lightShader.SetMat4f("projection", mainCamera.GetProjectionMatrix());
		glm::mat4 lightModel = glm::mat4(1.0f);
		float t = glfwGetTime();
		lightModel = glm::rotate(lightModel, 0.5f * t, glm::vec3(0.0f,1.0f,0.0f));
		lightModel = glm::translate(lightModel, lightPos);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));

		lightShader.SetMat4f("model", lightModel);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	});

	//------------ ��Ⱦ
	auto renderOp = new OpQueue;
	(*renderOp) << clearOp << cubeOp << sphereOp << lightOp;
	//------------- ĩβ
	auto endOp = new LambdaOp([]() {
		glfwSwapBuffers(Glfw::GetInstance()->GetWindow());
		glfwPollEvents();
	});
	//-------------
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
	printf("config.out read success\nRootPath is %s\n", config->GetStrPtr("RootPath")->c_str());
	GStorage<Config *>::GetInstance()->Register(str_MainCamera, config);
	return config;
}