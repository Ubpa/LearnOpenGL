#include <GLFW/Glfw.h>

using namespace LOGL;
using namespace Ubpa;

void processInput();

int main(int argc, char ** argv) {
	size_t width = 800, height = 600;
	char title[] = "01_CreateWindow";
	Glfw::GetInstance()->Init(width, height, title);
	//------------
	LambdaOp processInputOp(processInput);

	LambdaOp renderOp([&]() {
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	});

	LambdaOp endOp([]() {
		//------------ ��������
		glfwSwapBuffers(Glfw::GetInstance()->GetWindow());
		//------------ ��ȡ�¼�
		glfwPollEvents();
	});

	//OpQueue opQueue(); <--- ����������Ϊ������һ������
	OpQueue opQueue;
	opQueue << processInputOp << renderOp << endOp;
	//------------
	Glfw::GetInstance()->Run(opQueue);
	//------------
	Glfw::GetInstance()->Terminate();
	return 0;
}

void processInput() {
	if (Glfw::GetInstance()->GetKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
		Glfw::GetInstance()->CloseWindow();
}