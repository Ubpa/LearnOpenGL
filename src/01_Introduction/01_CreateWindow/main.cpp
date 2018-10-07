#include <GLFW/Glfw.h>

int main(int argc, char ** argv) {
	size_t width = 800, height = 600;
	char title[] = "01_CreateWindow";
	Glfw::Instance()->Init(width, height, title);
	//------------
	Glfw::Instance()->Run();
	Glfw::Instance()->Terminate();
	return 0;
}