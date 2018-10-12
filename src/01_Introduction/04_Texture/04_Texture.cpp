#include <ROOT_PATH.h>
#include <GLFW/Glfw.h>
#include <Shader.h>
#include <Utility/Image.h>

using namespace LOGL;
using namespace Ubpa;
using namespace std;

void processInput();

int main(int argc, char ** argv) {
	cout
		<< "*****************************************" << endl
		<< "* 1. Press '1' to set PolygonMode[FILL] *" << endl
		<< "* 2. Press '2' to set PolygonMode[LINE] *" << endl
		<< "*****************************************" << endl;
	//-------------
	string chapter = "01_Introduction";
	string subchapter = "04_Texture";
	//------------
	size_t windowWidth = 1024, windowHeight = 768;
	string windowTitle = chapter + "/" + subchapter;
	Glfw::GetInstance()->Init(windowWidth, windowHeight, windowTitle.c_str());
	//------------
	float vertices0[] = {
		//位置              //颜色
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // 右上角
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // 右下角
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // 左下角
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f  // 左上角
	};
	float vertices1[] = {
		//位置              //颜色
		-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下角
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // 左上角
		-1.0f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f  // 左
	};
	size_t indices[] = { // 注意索引从0开始! 
		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};
	float texCoords[] = {
	0.0f, 0.0f, // 左下角
	1.0f, 0.0f, // 右下角
	0.5f, 1.0f // 上中
	};
	//------------
	size_t VBO[2];
	glGenBuffers(2, VBO);
	size_t VAO[2];
	glGenVertexArrays(2, VAO);
	size_t EBO;
	glGenBuffers(1, &EBO);
	//------------ VAO[0]
	glBindVertexArray(VAO[0]);
	//------------
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices0), vertices0, GL_STATIC_DRAW);
	//------------
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//------------
	size_t texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//------------
	string imgName = string(ROOT_PATH) + "/data/textures/containers.jpg";
	Image img(imgName.c_str());
	/*
	@1 纹理目标
	@2 多级渐远纹理的级别 （0 为基本级别)
	@3 纹理格式
	@4 width
	@5 height
	@6 0 (历史遗留问题)
	@7 源图格式
	@8 源图类型
	@9 图像数据
	*/
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.GetWidth(), img.GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, img.GetData());
	// 为当前绑定的纹理自动生成所有需要的多级渐远纹理。
	glGenerateMipmap(GL_TEXTURE_2D);
	img.Free();
	//------------ VAO[1]
	glBindVertexArray(VAO[1]);
	//------------
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	//------------
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//------------
	string prefix = string(ROOT_PATH) + "/data/shaders/" + chapter + "/" + subchapter + "/";
	string vsF = prefix + subchapter + ".vs";
	string fsF = prefix + subchapter + ".fs";
	Shader shader(vsF.c_str(), fsF.c_str());
	if (!shader.IsValid()) {
		cout << "Shader is not Valid\n";
		return 1;
	}
	//------------
	LambdaOp processInputOp(processInput);

	LambdaOp renderOp([&]() {
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//------------ 
		glBindVertexArray(VAO[0]);
		shader.Use();
		float greenValue = (sinf(5.0f*glfwGetTime()) / 2.0f) + 0.5f;
		shader.SetVec4f("ourColor", 0.0f, greenValue, 0.0f, 1.0f);
		shader.SetVec3f("offset", sinf(5.0f*glfwGetTime()) * 0.5f, 0, 0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
		//------------
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	});

	LambdaOp endOp([]() {
		glfwSwapBuffers(Glfw::GetInstance()->GetWindow());
		glfwPollEvents();
	});

	//OpQueue opQueue(); <--- 编译器会以为声明了一个函数
	OpQueue opQueue; 
	opQueue << processInputOp << renderOp << endOp;
	//------------
	Glfw::GetInstance()->Run(opQueue);
	//------------
	Glfw::GetInstance()->Terminate();
	return 0;
}

void processInput(){
	if (Glfw::GetInstance()->GetKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
		Glfw::GetInstance()->CloseWindow();
	if (Glfw::GetInstance()->GetKey(GLFW_KEY_1) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (Glfw::GetInstance()->GetKey(GLFW_KEY_2) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}