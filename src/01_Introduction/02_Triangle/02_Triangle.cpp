#include <ROOT_PATH.h>
#include <GLFW/Glfw.h>
#include <Shader.h>

using namespace LOGL;
using namespace Ubpa;
using namespace std;

void processInput();

int main(int argc, char ** argv) {
	cout << "1. Press '1' to set PolygonMode[FILL]\n"
		 << "2. Press '2' to set PolygonMode[LINE]\n";
	//------------
	size_t width = 800, height = 600;
	char title[] = "02_Triangle";
	Glfw::GetInstance()->Init(width, height, title);
	//------------
	float vertices[] = {
		0.5f, 0.5f, 0.0f,   // ���Ͻ�
		0.5f, -0.5f, 0.0f,  // ���½�
		-0.5f, -0.5f, 0.0f, // ���½�
		-0.5f, 0.5f, 0.0f   // ���Ͻ�
	};
	size_t indices[] = { // ע��������0��ʼ! 
	0, 1, 3, // ��һ��������
	1, 2, 3  // �ڶ���������
	};
	//------------ 0. ���� VBO , VAO �� EBO
	size_t VBO;
	glGenBuffers(1, &VBO);
	size_t VAO;
	glGenVertexArrays(1, &VAO);
	size_t EBO[2];
	glGenBuffers(2, EBO);
	//------------ 1. �� VAO
	glBindVertexArray(VAO);
	//------------ 2. �� VBO, �����������͵� VBO ��
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	/*
	* GL_STATIC_DRAW �����ݲ���򼸺�����ı䡣
	* GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶࡣ
	* GL_STREAM_DRAW ������ÿ�λ���ʱ����ı䡣
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//------------ 3. �� EBO, �����������͵� EBO ��
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(size_t), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(size_t), &(indices[0])+3, GL_STATIC_DRAW);
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);
	//------------ 5. ������ɫ��
	string prefix = "/data/shaders/01_Introduction/02_Triangle/";
	string vsF = string(ROOT_PATH) + prefix + "02_Triangle.vs";
	string fsRedF = string(ROOT_PATH) + prefix + "02_Triangle_Red.fs";
	string fsBlueF = string(ROOT_PATH) + prefix + "02_Triangle_Blue.fs";
	Shader shaderRed(vsF.c_str(), fsRedF.c_str());
	Shader shaderBlue(vsF.c_str(), fsBlueF.c_str());
	if (!shaderRed.IsValid() || !shaderBlue.IsValid()) {
		cout << "Shader is not Valid\n";
		return 1;
	}
	//------------ 6. ������Ⱦѭ��
	LambdaOperation processInputOp(processInput, true);

	LambdaOperation renderOp([&]() {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(VAO);
		//------------ Red
		shaderRed.Use();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
		/*
		* @1 ����ģʽ
		* @2 ���ƵĶ������
		* @3 ��������
		* @4 ƫ����
		*/
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);
		//------------ Blue
		shaderBlue.Use();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);
		glfwSwapBuffers(Glfw::GetInstance()->GetWindow());
		glfwPollEvents();
	}, true);

	OperationQueue opQueue(true);
	opQueue.push(&processInputOp);
	opQueue.push(&renderOp);
	//------------
	Glfw::GetInstance()->Run(&opQueue);
	//------------
	Glfw::GetInstance()->Terminate();
	//------------
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