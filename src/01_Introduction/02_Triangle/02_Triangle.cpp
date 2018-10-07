#include <GLFW/Glfw.h>

using namespace LOGL;
using namespace std;

int main(int argc, char ** argv) {
	size_t width = 800, height = 600;
	char title[] = "01_CreateWindow";
	Glfw::GetInstance()->Init(width, height, title);
	//------------
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};
	size_t VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//GL_STATIC_DRAW ：数据不会或几乎不会改变。
	//GL_DYNAMIC_DRAW：数据会被改变很多。
	//GL_STREAM_DRAW ：数据每次绘制时都会改变。
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//------------
	Glfw::GetInstance()->Run();
	//------------
	Glfw::GetInstance()->Terminate();
	return 0;
}