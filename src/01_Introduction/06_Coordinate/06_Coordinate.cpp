#include <ROOT_PATH.h>
#include <GLFW/Glfw.h>
#include <Shader.h>
#include <Utility/Image.h>
#include <Utility/Storage.h>
#include <Utility/InfoLambdaOp.h>

using namespace LOGL;
using namespace std;
using namespace Ubpa;

class RegisterInput : public Operation{
public:
	RegisterInput(size_t textureID, size_t windowWidth, size_t windowHeight, glm::mat4 * projection, bool isHold = true)
		:Operation(isHold), textureID(textureID), windowWidth(windowWidth), windowHeight(windowHeight), projection(projection){
	}
	void Run();
private:
	size_t textureID;
	//------------
	struct Info1 {
		glm::mat4 * projection;
		float width;
		float height;
	};
	glm::mat4 * projection;
	size_t windowWidth;
	size_t windowHeight;
};

int main(int argc, char ** argv) {
	string chapter = "01_Introduction";
	string subchapter = "06_Coordinate";
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
	glm::mat4 model(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 view(1.0f);
	// 注意，我们将矩阵向我们要进行移动场景的反方向移动。
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
	//------------
	
 	auto registerInputOp = new RegisterInput(1, windowWidth, windowHeight, &projection, false);

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
		shader.SetMat4f("model", model);
		shader.SetMat4f("view", view);
		shader.SetMat4f("projection", projection);
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
	// Close Window
	auto closeWindowOp = new LambdaOp([]() {
		Glfw::GetInstance()->CloseWindow();
	});
	EventManager::GetInstance()->RegisterOp(GLFW_KEY_ESCAPE, closeWindowOp);

	// Polygon Mode
	//------------ GLFW_KEY_1
	auto polygonModeFillOp = new LambdaOp([]() {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	});
	EventManager::GetInstance()->RegisterOp(GLFW_KEY_1, polygonModeFillOp);
	//------------ GLFW_KEY_2
	auto polygonModeLineOp = new LambdaOp([]() {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	});
	EventManager::GetInstance()->RegisterOp(GLFW_KEY_2, polygonModeLineOp);

	// Texture Warp
	size_t info0 = GL_TEXTURE0 + textureID;
	//------------ GLFW_KEY_3
	auto textWarpReapeatOp = new InfoLambdaOp<size_t>("textWarpReapeatOp", info0, []() {
		auto textWarpReapeatOp = InfoLambdaOp<size_t>::GetFromStorage("textWarpReapeatOp");
		glBindTexture(GL_TEXTURE_2D, textWarpReapeatOp->GetInfo());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	});
	EventManager::GetInstance()->RegisterOp(GLFW_KEY_3, textWarpReapeatOp);
	//------------ GLFW_KEY_4
	auto textWarpMirrorReapeatOp = new InfoLambdaOp<size_t>("textWarpMirrorReapeatOp", info0, []() {
		auto textWarpMirrorReapeatOp = InfoLambdaOp<size_t>::GetFromStorage("textWarpMirrorReapeatOp");
		glBindTexture(GL_TEXTURE_2D, textWarpMirrorReapeatOp->GetInfo());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	});
	EventManager::GetInstance()->RegisterOp(GLFW_KEY_4, textWarpMirrorReapeatOp);
	//------------ GLFW_KEY_5
	auto textWarpClamp2EdgeOp = new InfoLambdaOp<size_t>("textWarpClamp2EdgeOp", info0, []() {
		auto textWarpClamp2EdgeOp = InfoLambdaOp<size_t>::GetFromStorage("textWarpClamp2EdgeOp");
		glBindTexture(GL_TEXTURE_2D, textWarpClamp2EdgeOp->GetInfo());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	});
	EventManager::GetInstance()->RegisterOp(GLFW_KEY_5, textWarpClamp2EdgeOp);
	//------------ GLFW_KEY_6
	auto textWarpClamp2BodderOp = new InfoLambdaOp<size_t>("textWarpClamp2BodderOp", info0, []() {
		auto textWarpClamp2BodderOp = InfoLambdaOp<size_t>::GetFromStorage("textWarpClamp2BodderOp");
		glBindTexture(GL_TEXTURE_2D, textWarpClamp2BodderOp->GetInfo());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	});
	EventManager::GetInstance()->RegisterOp(GLFW_KEY_6, textWarpClamp2BodderOp);

	// Projection
	Info1 info1 = { projection, (float)windowWidth , (float)windowHeight };
	//------------ GLFW_KEY_7
	auto perspectiveOp = new InfoLambdaOp<Info1>("perspectiveOp", info1, []() {
		auto perspectiveOp = InfoLambdaOp<Info1>::GetFromStorage("perspectiveOp");
		auto & info = perspectiveOp->GetInfo();
		(*info.projection) = glm::perspective(glm::radians(45.0f), info.width / info.height, 0.1f, 100.0f);
	});
	EventManager::GetInstance()->RegisterOp(GLFW_KEY_7, perspectiveOp);
	//------------ GLFW_KEY_8
	auto orthoOp = new InfoLambdaOp<Info1>("orthoOp", info1, []() {
		auto orthoOp = InfoLambdaOp<Info1>::GetFromStorage("orthoOp");
		auto & info = orthoOp->GetInfo();
		(*info.projection) = glm::ortho(-1.0f, 1.0f, -info.height / info.width, info.height / info.width, 0.01f, 100.0f);
	});
	EventManager::GetInstance()->RegisterOp(GLFW_KEY_8, orthoOp);

	//------------

	cout << endl
		<< "* 1. Press '1' to set PolygonMode[FILL]" << endl
		<< "* 2. Press '2' to set PolygonMode[LINE]" << endl
		<< "* 3. Press '3' to set TEXTURE_WRAP[REPEAT]" << endl
		<< "* 4. Press '4' to set TEXTURE_WRAP[MIRRORED_REPEAT]" << endl
		<< "* 5. Press '5' to set TEXTURE_WRAP[CLAMP_TO_EDGE]" << endl
		<< "* 6. Press '6' to set TEXTURE_WRAP[CLAMP_TO_BORDER]" << endl
		<< "* 7. Press '7' to set projection[perspective]" << endl
		<< "* 8. Press '8' to set projection[ortho]" << endl
		<< "* 9. Press 'ESC' to close exe" << endl << endl;
}