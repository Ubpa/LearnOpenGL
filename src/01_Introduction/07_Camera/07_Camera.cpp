#include <ROOT_PATH.h>

#include <Utility/Image.h>
#include <Utility/Storage.h>
#include <Utility/InfoLambdaOp.h>

#include <Shader.h>
#include <Camera.h>

#include "RegisterInput.h"

using namespace LOGL;
using namespace std;
using namespace Ubpa;

//------------

int main(int argc, char ** argv) {
	string chapter = "01_Introduction";
	string subchapter = "07_Camera";
	//------------ 窗口
	size_t windowWidth = 1024, windowHeight = 576;
	float ratioWH = (float)windowWidth / (float)windowHeight;
	string windowTitle = chapter + "/" + subchapter;
	Glfw::GetInstance()->Init(windowWidth, windowHeight, windowTitle.c_str());
	Glfw::GetInstance()->LockCursor();
	//------------ 顶点数据
	float vertices[] = {
		-0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,  1.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,  1.5f,  1.5f,
		 0.5f,  0.5f, -0.5f,  1.5f,  1.5f,
		-0.5f,  0.5f, -0.5f, -0.5f,  1.5f,
		-0.5f, -0.5f, -0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,  1.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,  1.5f,  1.5f,
		 0.5f,  0.5f,  0.5f,  1.5f,  1.5f,
		-0.5f,  0.5f,  0.5f, -0.5f,  1.5f,
		-0.5f, -0.5f,  0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f,  0.5f,  1.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,  1.5f,  1.5f,
		-0.5f, -0.5f, -0.5f, -0.5f,  1.5f,
		-0.5f, -0.5f, -0.5f, -0.5f,  1.5f,
		-0.5f, -0.5f,  0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f,  0.5f,  1.5f, -0.5f,

		 0.5f,  0.5f,  0.5f,  1.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,  1.5f,  1.5f,
		 0.5f, -0.5f, -0.5f, -0.5f,  1.5f,
		 0.5f, -0.5f, -0.5f, -0.5f,  1.5f,
		 0.5f, -0.5f,  0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,  1.5f, -0.5f,

		-0.5f, -0.5f, -0.5f, -0.5f,  1.5f,
		 0.5f, -0.5f, -0.5f,  1.5f,  1.5f,
		 0.5f, -0.5f,  0.5f,  1.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,  1.5f, -0.5f,
		-0.5f, -0.5f,  0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f, -0.5f,  1.5f,

		-0.5f,  0.5f, -0.5f, -0.5f,  1.5f,
		 0.5f,  0.5f, -0.5f,  1.5f,  1.5f,
		 0.5f,  0.5f,  0.5f,  1.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,  1.5f, -0.5f,
		-0.5f,  0.5f,  0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f, -0.5f,  1.5f,
	};
	//------------ 模型数据
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	//------------
	size_t VBO;
	glGenBuffers(1, &VBO);
	size_t VAO;
	glGenVertexArrays(1, &VAO);
	//------------
	glBindVertexArray(VAO);
	//------------
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//------------ 属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//------------ 纹理
	size_t texture[2];
	glGenTextures(2, texture);
	string imgName[2] = {
		string(ROOT_PATH) + "/data/textures/container.jpg",
		string(ROOT_PATH) + "/data/textures/awesomeface.png"
	};
	Image img[2];
	GLenum mode[2] = { GL_RGB, GL_RGBA };
	bool flip[2] = { false, true };
	
	for (size_t i = 0; i < 2; i++) {
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		// 为当前绑定的纹理对象设置环绕、过滤方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		img[i].Load(imgName[i].c_str(), flip[i]);
		if (!img[i].IsValid()) {
			cout << "Failed to load texture[" << imgName[i] << "]\n";
			return 1;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img[i].GetWidth(), img[i].GetHeight(), 0, mode[i], GL_UNSIGNED_BYTE, img[i].GetData());
		// 为当前绑定的纹理自动生成所有需要的多级渐远纹理。
		glGenerateMipmap(GL_TEXTURE_2D);
		img[i].Free();
	}
	//------------ 着色器
	string prefix = string(ROOT_PATH) + "/data/shaders/" + chapter + "/" + subchapter + "/";
	string vsF = prefix + subchapter + ".vs";
	string fsF = prefix + subchapter + ".fs";
	Shader shader(vsF.c_str(), fsF.c_str());
	if (!shader.IsValid()) {
		cout << "Shader is not Valid\n";
		return 1;
	}
	//------------ 绑定纹理
	shader.Use();
	shader.SetInt("texture0", 0);
	shader.SetInt("texture1", 1);
	//------------ 矩阵
	Camera mainCamera(ratioWH, 0.1f, 100.0f, glm::vec3(0.0f,0.0f,4.0f));
	Storage<Camera *>::GetInstance()->Register("mainCamera", &mainCamera);
	//------------ 输入注册
 	auto registerInputOp = new RegisterInput(1, false);
	//------------- 时间
	float deltaTime = 0.0f; // 当前帧与上一帧的时间差
	Storage<float *>::GetInstance()->Register("deltaTime", &deltaTime);
	float lastFrame = 0.0f; // 上一帧的时间
	auto timeOp = new LambdaOp([&]() {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	});
	//------------ 渲染
	auto renderOp = new LambdaOp([&]() {
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//------------ 
		glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活纹理单元
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glActiveTexture(GL_TEXTURE1); // 在绑定纹理之前先激活纹理单元
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		//------------
		shader.Use();
		glBindVertexArray(VAO);
		//------------
		shader.SetMat4f("view", glm::value_ptr(mainCamera.GetViewMatrix()));
		shader.SetMat4f("projection", glm::value_ptr(mainCamera.GetProjectionMatrix()));

		for (size_t i = 0; i < 10; i++) {
			glm::mat4 model(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = (float)glfwGetTime() * 50.0f + 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.SetMat4f("model", glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	});
	//-------------
	auto endOp = new LambdaOp ([]() {
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

//------------

