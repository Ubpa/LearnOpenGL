#include <GLFW/Glfw.h>
#include <Utility/LambdaOp.h>

#include <iostream>

using namespace LOGL;
using namespace Ubpa;
using namespace std;

void registerInput();

int main(int argc, char ** argv) {
	size_t width = 800, height = 600;
	string chapter = "01_Introduction";
	string subchapter = "01_Window";
	//------------
	size_t windowWidth = 1024, windowHeight = 768;
	string windowTitle = chapter + "/" + subchapter;
	Glfw::GetInstance()->Init(windowWidth, windowHeight, windowTitle.c_str());
	//------------
	registerInput();

	auto initOp = new LambdaOp(registerInput, false);

	auto renderOp = new LambdaOp([&]() {
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	});

	auto endOp = new LambdaOp([]() {
		//------------ 交换缓冲
		glfwSwapBuffers(Glfw::GetInstance()->GetWindow());
		//------------ 拉取事件
		glfwPollEvents();
	});

	auto opQueue = new OpQueue;
	(*opQueue) << initOp << renderOp << endOp;
	//------------
	Glfw::GetInstance()->Run(opQueue);
	//------------
	Glfw::GetInstance()->Terminate();
	return 0;
}

void registerInput() {
	EventManager::GetInstance()->Register(EventManager::KEYBOARD | GLFW_KEY_ESCAPE, []() {
		if (Glfw::GetInstance()->GetKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
			Glfw::GetInstance()->CloseWindow();
	});
	EventManager::GetInstance()->Register(EventManager::KEYBOARD | GLFW_KEY_1, []() {
		if(Glfw::GetInstance()->GetKey(GLFW_KEY_1) == GLFW_PRESS)
			printf("press 1\n");
	});
	cout << endl
		<< "* 1. Press '1' to print \"press 1\"" << endl
		<< "* 2. Press 'ESC' to close exe" << endl << endl;
}