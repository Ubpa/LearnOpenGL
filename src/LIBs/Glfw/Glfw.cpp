#include <GLFW/Glfw.h>

using namespace LOGL;
using namespace Ubpa;
using namespace std;

Glfw::Glfw():window(NULL){ }

//------------

void Glfw::Init(size_t width, size_t height, const char * title){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//mac os
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//------------
	GenWindow(width, height, title);
	LoadGL();
	glViewport(0, 0, width, height);
	glfwSetKeyCallback(Glfw::GetInstance()->GetWindow(), [](GLFWwindow * window, int key, int scanCode, int state, int mods) {
		Ubpa::EventManager::GetInstance()->Response(key);
	});
	glEnable(GL_DEPTH_TEST);
}

void Glfw::Terminate() { glfwTerminate(); }

GLFWwindow * Glfw::GetWindow() { return window; }

void Glfw::Run(Ptr<Operation> & op) {
	if (window == NULL)
		Init();
	//------------
	bool loop = op != NULL;
	while (!glfwWindowShouldClose(window)) {
		if (loop) {
			op->Run();
			if (!op->IsHold())
				loop = false;
		}
	}
}

void Glfw::Run(Operation * op) {
	Run(Ptr<Operation>(op, Operation::Delete));
}

int Glfw::GetKey(int key) { return glfwGetKey(window, key); }

void Glfw::CloseWindow() { glfwSetWindowShouldClose(window, true); }

//------------

Glfw* Glfw::instance = new Glfw();

Glfw* Glfw::GetInstance(){ return instance; }

//------------

void Glfw::CB_FrameBuffSize(GLFWwindow* window, int width, int height){
	glViewport(0, 0, width, height);
}

void Glfw::GenWindow(size_t width, size_t height, const char * title) {
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		Terminate();
		exit(1);
	}
	//------------
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, CB_FrameBuffSize);
}

void Glfw::LoadGL() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(1);
	}
}