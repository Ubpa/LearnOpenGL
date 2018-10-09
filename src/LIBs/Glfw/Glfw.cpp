#include <GLFW/Glfw.h>

using namespace LOGL;
using namespace std;

Glfw::Glfw():window(NULL){ }
//------------
void Glfw::Init(size_t width, size_t height, char * title){
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
}
void Glfw::Terminate() { glfwTerminate(); }
GLFWwindow * Glfw::GetWindow() { return window; }
void Glfw::Run(Ubpa::Operation * operation) {
	if (window == NULL)
		Init();
	while (!glfwWindowShouldClose(window))
	{
		if (operation != NULL)
			(*operation)();
	}
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
void Glfw::GenWindow(size_t width, size_t height, char * title) {
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		Terminate();
		exit(1);
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, CB_FrameBuffSize);
}
void Glfw::LoadGL() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(1);
	}
}