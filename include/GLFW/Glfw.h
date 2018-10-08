#ifndef GLFW_H_
#define GLFW_H_
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Utility\Operation.h>
#include <iostream>

namespace LOGL {
	class Glfw {
	public:
		static Glfw * GetInstance();
		//------------
		void Init(size_t width = 800, size_t height = 600, char * title = "Title");
		void Terminate();
		//void SetCB_FrameBuffSize();
		template<typename T>
		void Run(T* operation = NULL);
		void Run();
		GLFWwindow * GetWindow();
	private:
		Glfw();
		Glfw(const Glfw&);
		Glfw& operator=(const Glfw&);
		//------------
		static void CB_FrameBuffSize(GLFWwindow* window, int width, int height);
		void GenWindow(size_t width = 800, size_t height = 600, char * title = NULL);
		void LoadGL();
		//------------
		GLFWwindow * window;
		static Glfw * instance;
	};

	template<typename T>
	void Glfw::Run(T * operation) {
		if (window == NULL)
			Init();
		while (!glfwWindowShouldClose(window))
		{
			if (operation != NULL)
				(*operation)();
		}
	}
}

#endif