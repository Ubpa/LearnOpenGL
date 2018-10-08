#include <ROOT_PATH.h>
#include <GLFW/Glfw.h>
#include <Shader.h>

using namespace LOGL;
using namespace std;

int main(int argc, char ** argv) {
	size_t width = 800, height = 600;
	char title[] = "02_Triangle";
	Glfw::GetInstance()->Init(width, height, title);
	//------------
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
	//------------ 0. ���� VBO �� VAO
	size_t VBO;
	glGenBuffers(1, &VBO);
	size_t VAO;
	glGenVertexArrays(1, &VAO);
	//------------ 1. �� VAO
	glBindVertexArray(VAO);
	//------------ 2. �� VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//------------ 3. �����������͵� VBO ��
	/*
	* GL_STATIC_DRAW �����ݲ���򼸺�����ı䡣
	* GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶࡣ
	* GL_STREAM_DRAW ������ÿ�λ���ʱ����ı䡣
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//------------ 4. �󶨶�������ָ��
	/*
	* @1 Location : 0
	* @2 Attrib size : 3
	* @3 Type : float
	* @4 Normalize : false
	*     - signed type -> (-1, 1)
	*     - unsigned type -> (0, 1)
	* @5 Stride : 3 * sizeof(float)
	* @6 Offset : 0
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//------------ 5. ��ɫ��
	string prefix = "/data/shaders/01_Introduction/02_Triangle/";
	string vsF = string(ROOT_PATH) + prefix + "02_Triangle.vs";
	string fsF = string(ROOT_PATH) + prefix + "02_Triangle.fs";
	Shader shader(vsF.c_str(), fsF.c_str());
	if (!shader.IsValid()) {
		cout << "Shader Error\n";
		return 1;
	}
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	//------------
	Glfw::GetInstance()->Run(&([&]() {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shader.Use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(Glfw::GetInstance()->GetWindow());
		glfwPollEvents();
	}));
	//------------
	Glfw::GetInstance()->Terminate();
	return 0;
}