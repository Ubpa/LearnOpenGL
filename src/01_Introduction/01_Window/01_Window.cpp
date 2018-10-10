#include <GLFW/Glfw.h>

using namespace LOGL;
using namespace Ubpa;

void processInput();

int main(int argc, char ** argv) {
	size_t width = 800, height = 600;
	char title[] = "01_CreateWindow";
	Glfw::GetInstance()->Init(width, height, title);
	//------------
	LambdaOperation processInputOp(processInput);

	LambdaOperation renderOp([&]() {
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	});

	LambdaOperation endOp([]() {
		//------------ ��������
		glfwSwapBuffers(Glfw::GetInstance()->GetWindow());
		//------------ ��ȡ�¼�
		glfwPollEvents();
	});

	//OperationQueue opQueue(); <--- ����������Ϊ������һ������
	OperationQueue opQueue;
	opQueue.push(processInputOp);
	opQueue.push(renderOp);
	opQueue.push(endOp);
	//------------
	Glfw::GetInstance()->Run([]() {});
	//------------
	Glfw::GetInstance()->Terminate();
	return 0;
}

void processInput() {
	if (Glfw::GetInstance()->GetKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
		Glfw::GetInstance()->CloseWindow();
}