#include <GLFW/Glfw.h>

using namespace LOGL;

int main(int argc, char ** argv) {
	size_t width = 800, height = 600;
	char title[] = "01_CreateWindow";
	Glfw::GetInstance()->Init(width, height, title);
	//------------
	Glfw::GetInstance()->Run();
	Glfw::GetInstance()->Terminate();
	return 0;
}