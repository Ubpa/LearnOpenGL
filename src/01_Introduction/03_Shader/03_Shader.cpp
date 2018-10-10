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
		0.5f, 0.5f, 0.0f,   // ���Ͻ�
		0.5f, -0.5f, 0.0f,  // ���½�
		-0.5f, -0.5f, 0.0f, // ���½�
		-0.5f, 0.5f, 0.0f,   // ���Ͻ�
	};
	float vertices1[] = {
		//λ��				 //��ɫ
		-0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // ���½�
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // ���Ͻ�
		-1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f  // ��
	};
	size_t indices[] = { // ע��������0��ʼ! 
		0, 1, 3, // ��һ��������
		1, 2, 3  // �ڶ���������
	};
	//------------ 0. ���� VBO , VAO �� EBO
	size_t VBO[2];
	glGenBuffers(2, VBO);
	size_t VAO[2];
	glGenVertexArrays(2, VAO);
	size_t EBO;
	glGenBuffers(1, &EBO);
	//------------ 1. �� VAO
	glBindVertexArray(VAO[0]);
	//------------ 2. �� VBO, �����������͵� VBO ��
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	/*
	* GL_STATIC_DRAW �����ݲ���򼸺�����ı䡣
	* GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶࡣ
	* GL_STREAM_DRAW ������ÿ�λ���ʱ����ı䡣
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices0), vertices0, GL_STATIC_DRAW);
	//------------ 3. �� EBO, �����������͵� EBO ��
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
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
	//------------ 5. �ظ�1, 2 �� 4, ���趨һ�� VAO
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	// λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// ��ɫ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//------------ 5. ������ɫ��
	string prefix = string(ROOT_PATH) + "/data/shaders/01_Introduction/03_Shader/";
	string vsF = prefix + "03_Shader.vs";
	string fsF = prefix + "03_Shader.fs";
	//string fsBlueF = prefix + "02_Triangle_Blue.fs";
	Shader shader(vsF.c_str(), fsF.c_str());
	//Shader shaderBlue(vsF.c_str(), fsBlueF.c_str());
	if (!shader.IsValid() /*|| !shaderBlue.IsValid()*/) {
		cout << "Shader is not Valid\n";
		return 1;
	}
	//------------ 6. ������Ⱦѭ��
	LambdaOperation processInputOp(processInput, true);

	LambdaOperation renderOp([&]() {
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//------------ 
		glBindVertexArray(VAO[0]);
		shader.Use();
		float greenValue = (sinf(5.0f*glfwGetTime()) / 2.0f) + 0.5f;
		shader.SetVec4f("ourColor", 0.0f, greenValue, 0.0f, 1.0f);
		shader.SetVec3f("offset", sinf(5.0f*glfwGetTime()) * 0.5f, 0, 0);
		/*
		* @1 ����ģʽ
		* @2 ���ƵĶ������
		* @3 ��������
		* @4 ƫ����
		*/
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
		//------------
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//------------ ��������
		glfwSwapBuffers(Glfw::GetInstance()->GetWindow());
		//------------ ��ȡ�¼�
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