#include <ROOT_PATH.h>
#include <GLFW/Glfw.h>
#include <Shader.h>

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
	//------------
	size_t width = 800, height = 600;
	char title[] = "03_Shader";
	Glfw::GetInstance()->Init(width, height, title);
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
	string prefix = string(ROOT_PATH) + "/data/shaders/01_Introduction/" + title + "/";
	string vsF = prefix + title + ".vs";
	string fsF = prefix + title + ".fs";
	Shader shader(vsF.c_str(), fsF.c_str());
	if (!shader.IsValid()) {
		cout << "Shader is not Valid\n";
		return 1;
	}
	//------------ 7. 设置渲染循环
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