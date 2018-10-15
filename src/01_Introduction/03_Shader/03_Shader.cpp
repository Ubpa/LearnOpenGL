#include <ROOT_PATH.h>
#include <GLFW/Glfw.h>
#include <Shader.h>
#include <Utility/LambdaOp.h>

using namespace LOGL;
using namespace Ubpa;
using namespace std;

void registerInput();

int main(int argc, char ** argv) {
	size_t width = 800, height = 600;
	string chapter = "01_Introduction";
	string subchapter = "03_Shader";
	//------------
	size_t windowWidth = 1024, windowHeight = 768;
	string windowTitle = chapter + "/" + subchapter;
	Glfw::GetInstance()->Init(windowWidth, windowHeight, windowTitle.c_str());
	//------------
	float vertices0[] = {
		0.5f, 0.5f, 0.0f,   // 右上角
		0.5f, -0.5f, 0.0f,  // 右下角
		-0.5f, -0.5f, 0.0f, // 左下角
		-0.5f, 0.5f, 0.0f,   // 左上角
	};
	float vertices1[] = {
		//位置				 //颜色
		-0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // 左下角
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // 左上角
		-1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f  // 左
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
	//------------ 0. 生成 VBO , VAO 和 EBO
	size_t VBO[2];
	glGenBuffers(2, VBO);
	size_t VAO[2];
	glGenVertexArrays(2, VAO);
	size_t EBO;
	glGenBuffers(1, &EBO);
	//------------ 1. 绑定 VAO
	glBindVertexArray(VAO[0]);
	//------------ 2. 绑定 VBO, 将顶点数据送到 VBO 中
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices0), vertices0, GL_STATIC_DRAW);
	//------------ 3. 绑定 EBO, 将索引数据送到 EBO 中
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//------------ 4. 绑定顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);
	//------------ 5. 重复1, 2 和 4, 再设定一个 VAO
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	// 位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//------------ 6. 编译着色器
	string prefix = string(ROOT_PATH) + "/data/shaders/" + chapter + "/" + subchapter + "/";
	string vsF = prefix + subchapter + ".vs";
	string fsF = prefix + subchapter + ".fs";
	Shader shader(vsF.c_str(), fsF.c_str());
	if (!shader.IsValid()) {
		cout << "Shader is not Valid\n";
		return 1;
	}
	//------------ 7. 设置渲染循环
	auto initOp = new LambdaOp(registerInput, false);

	auto renderOp = new LambdaOp([&]() {
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//------------ 
		glBindVertexArray(VAO[0]);
		shader.Use();
		float t = static_cast<float>(glfwGetTime());
		float greenValue = (sinf(5.0f*t) / 2.0f) + 0.5f;
		shader.SetVec4f("ourColor", 0.0f, greenValue, 0.0f, 1.0f);
		shader.SetVec3f("offset", sinf(5.0f*t) * 0.5f, 0, 0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
		//------------
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	});

	auto endOp = new LambdaOp([]() {
		glfwSwapBuffers(Glfw::GetInstance()->GetWindow());
		glfwPollEvents();
	});

	//OpQueue opQueue(); <--- 编译器会以为声明了一个函数
	auto opQueue = new OpQueue;
	(*opQueue) << initOp << renderOp << endOp;
	//------------
	Glfw::GetInstance()->Run(opQueue);
	//------------
	Glfw::GetInstance()->Terminate();
	return 0;
}

void registerInput(){
	EventManager::GetInstance()->Register(GLFW_KEY_ESCAPE, []() {
		if (Glfw::GetInstance()->GetKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
			Glfw::GetInstance()->CloseWindow();
	});
	EventManager::GetInstance()->Register(GLFW_KEY_1, []() {
		if (Glfw::GetInstance()->GetKey(GLFW_KEY_1) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	});
	EventManager::GetInstance()->Register(GLFW_KEY_2, []() {
		if (Glfw::GetInstance()->GetKey(GLFW_KEY_2) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	});

	cout << endl
		<< "* 1. Press '1' to set PolygonMode[FILL]" << endl
		<< "* 2. Press '2' to set PolygonMode[LINE]" << endl
		<< "* 3. Press 'ESC' to close exe" << endl << endl;
}