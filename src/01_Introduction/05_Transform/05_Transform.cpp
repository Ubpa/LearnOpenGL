#include <ROOT_PATH.h>
#include <GLFW/Glfw.h>
#include <Shader.h>
#include <Utility/Image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace LOGL;
using namespace Ubpa;
using namespace std;

class RegisterInput : public Operation{
public:
	RegisterInput(size_t textureID, bool isHold = true)
		:Operation(isHold), textureID(textureID) { }
	void Run();
private:
	size_t textureID;
};

int main(int argc, char ** argv) {
	string chapter = "01_Introduction";
	string subchapter = "05_Transform";
	//------------
	size_t windowWidth = 1024, windowHeight = 768;
	string windowTitle = chapter + "/" + subchapter;
	Glfw::GetInstance()->Init(windowWidth, windowHeight, windowTitle.c_str());
	//------------
	float vertices[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // 右上
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // 右下
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // 左上
	};
	size_t indices[] = { // 注意索引从0开始! 
		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};
	//------------
	size_t VBO;
	glGenBuffers(1, &VBO);
	size_t VAO;
	glGenVertexArrays(1, &VAO);
	size_t EBO;
	glGenBuffers(1, &EBO);
	//------------
	glBindVertexArray(VAO);
	//------------
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//------------
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//------------
	size_t texture[2];
	glGenTextures(2, texture);
	string imgName[2] = {
		string(ROOT_PATH) + "/data/textures/container.jpg",
		string(ROOT_PATH) + "/data/textures/awesomeface.png"
	};
	Image img[2];
	GLenum mode[2] = { GL_RGB, GL_RGBA };
	bool flip[2] = { false, true };
	//-------------
	for (size_t i = 0; i < 2; i++) {
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		// 为当前绑定的纹理对象设置环绕、过滤方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		img[i].Load(imgName[i].c_str(), flip[i]);
		if (!img[i].IsValid()) {
			cout << "Failed to load texture[" << imgName[i] << "]\n";
			return 1;
		}
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img[i].GetWidth(), img[i].GetHeight(), 0, mode[i], GL_UNSIGNED_BYTE, img[i].GetData());
		// 为当前绑定的纹理自动生成所有需要的多级渐远纹理。
		glGenerateMipmap(GL_TEXTURE_2D);
		img[i].Free();
	}
	//------------
	string prefix = string(ROOT_PATH) + "/data/shaders/" + chapter + "/" + subchapter + "/";
	string vsF = prefix + subchapter + ".vs";
	string fsF = prefix + subchapter + ".fs";
	Shader shader(vsF.c_str(), fsF.c_str());
	if (!shader.IsValid()) {
		cout << "Shader is not Valid\n";
		return 1;
	}
	//------------
	shader.Use();
	shader.SetInt("texture0", 0);
	shader.SetInt("texture1", 1);
	//------------
	
	auto registerInputOp = new RegisterInput(1, false);
	
	//-------------
	auto renderOp = new LambdaOp([&]() {
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//------------ 
		glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活纹理单元
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glActiveTexture(GL_TEXTURE1); // 在绑定纹理之前先激活纹理单元
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		//------------
		shader.Use();
		glBindVertexArray(VAO);
		//------------
		float t = (float)glfwGetTime();
		float greenValue = (sinf(4.0f*t) / 2.0f) + 0.5f;
		shader.SetVec4f("ourColor", 0.0f, greenValue, 0.0f, 1.0f);
		shader.SetVec3f("offset", sinf(2.0f*t) * 0.5f, 0, 0);
		//------------
		glm::mat4 trans(1.0f);
		trans = glm::scale(trans, glm::vec3(1.0f, 1.0f, 1.0f)*(sinf(t)*0.5f) + 1.0f);
		trans = glm::rotate(trans, t, glm::vec3(1.0f, 1.0f, 1.0f));
		shader.SetMat4f("transform", trans);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
	});
	//-------------
	auto endOp = new LambdaOp ([]() {
		glfwSwapBuffers(Glfw::GetInstance()->GetWindow());
		glfwPollEvents();
	});
	//-------------
	auto opQueue = new OpQueue;
	(*opQueue) << registerInputOp << renderOp << endOp;
	//------------
	Glfw::GetInstance()->Run(opQueue);
	//------------
	Glfw::GetInstance()->Terminate();
	return 0;
}

void RegisterInput::Run() {
	cout << "TextureID:" << textureID << endl;

	int keys[7] = {
		GLFW_KEY_ESCAPE, GLFW_KEY_1, GLFW_KEY_2, GLFW_KEY_3, GLFW_KEY_4, GLFW_KEY_5, GLFW_KEY_6
	};
	function<void()> ops[7] = {
		[]() { Glfw::GetInstance()->CloseWindow(); },
		[]() {glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); },
		[]() {glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); },
		[&]() {
			glBindTexture(GL_TEXTURE_2D, GL_TEXTURE0 + textureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		},
		[&]() {
			glBindTexture(GL_TEXTURE_2D, GL_TEXTURE0 + textureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		},
		[&]() {
			glBindTexture(GL_TEXTURE_2D, GL_TEXTURE0 + textureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		},
		[&]() {
			glBindTexture(GL_TEXTURE_2D, GL_TEXTURE0 + textureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		},
	};

	for (int i = 0; i < 7; i++)
		EventManager::GetInstance()->RegisterOp(keys[i], ops[i]);

	cout << endl
		<< "* 1. Press '1' to set PolygonMode[FILL]" << endl
		<< "* 2. Press '2' to set PolygonMode[LINE]" << endl
		<< "* 3. Press '3' to set TEXTURE_WRAP[REPEAT]" << endl
		<< "* 4. Press '4' to set TEXTURE_WRAP[MIRRORED_REPEAT]" << endl
		<< "* 5. Press '5' to set TEXTURE_WRAP[CLAMP_TO_EDGE]" << endl
		<< "* 6. Press '6' to set TEXTURE_WRAP[CLAMP_TO_BORDER]" << endl
		<< "* 7. Press 'ESC' to close exe" << endl << endl;

	//------------

	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	// 译注：下面就是矩阵初始化的一个例子，如果使用的是0.9.9及以上版本
	// 下面这行代码就需要改为:
	// glm::mat4 trans = glm::mat4(1.0f)
	// 之后将不再进行提示
	glm::mat4 trans(1.0f);
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	vec = trans * vec;
	std::cout << vec.x << vec.y << vec.z << std::endl;
}