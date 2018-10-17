#include <ROOT_PATH.h>
#include <GLFW/Glfw.h>
#include <LOGL/Shader.h>
#include <Utility/Image.h>
#include <Utility/GStorage.h>
#include <Utility/InfoLambdaOp.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

#include <Utility/Config.h>
#include <Utility/GStorage.h>
const string str_MainConfig = "MainConfig";

Config * DoConfig();

int main(int argc, char ** argv) {
	Config * config = DoConfig();
	string rootPath = *config->GetStrPtr("RootPath");
	string chapter = "01_Introduction";
	string subchapter = "06_Coordinate";
	//------------
	size_t windowWidth = 1024, windowHeight = 576;
	string windowTitle = chapter + "/" + subchapter;
	Glfw::GetInstance()->Init(windowWidth, windowHeight, windowTitle.c_str());
	//------------
	float vertices[] = {
	-0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f, -0.5f,  1.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,  1.5f,  1.5f,
	 0.5f,  0.5f, -0.5f,  1.5f,  1.5f,
	-0.5f,  0.5f, -0.5f, -0.5f,  1.5f,
	-0.5f, -0.5f, -0.5f, -0.5f, -0.5f,

	-0.5f, -0.5f,  0.5f, -0.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,  1.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,  1.5f,  1.5f,
	 0.5f,  0.5f,  0.5f,  1.5f,  1.5f,
	-0.5f,  0.5f,  0.5f, -0.5f,  1.5f,
	-0.5f, -0.5f,  0.5f, -0.5f, -0.5f,

	-0.5f,  0.5f,  0.5f,  1.5f, -0.5f,
	-0.5f,  0.5f, -0.5f,  1.5f,  1.5f,
	-0.5f, -0.5f, -0.5f, -0.5f,  1.5f,
	-0.5f, -0.5f, -0.5f, -0.5f,  1.5f,
	-0.5f, -0.5f,  0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f,  0.5f,  1.5f, -0.5f,

	 0.5f,  0.5f,  0.5f,  1.5f, -0.5f,
	 0.5f,  0.5f, -0.5f,  1.5f,  1.5f,
	 0.5f, -0.5f, -0.5f, -0.5f,  1.5f,
	 0.5f, -0.5f, -0.5f, -0.5f,  1.5f,
	 0.5f, -0.5f,  0.5f, -0.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,  1.5f, -0.5f,

	-0.5f, -0.5f, -0.5f, -0.5f,  1.5f,
	 0.5f, -0.5f, -0.5f,  1.5f,  1.5f,
	 0.5f, -0.5f,  0.5f,  1.5f, -0.5f,
	 0.5f, -0.5f,  0.5f,  1.5f, -0.5f,
	-0.5f, -0.5f,  0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f, -0.5f,  1.5f,

	-0.5f,  0.5f, -0.5f, -0.5f,  1.5f,
	 0.5f,  0.5f, -0.5f,  1.5f,  1.5f,
	 0.5f,  0.5f,  0.5f,  1.5f, -0.5f,
	 0.5f,  0.5f,  0.5f,  1.5f, -0.5f,
	-0.5f,  0.5f,  0.5f, -0.5f, -0.5f,
	-0.5f,  0.5f, -0.5f, -0.5f,  1.5f,
	};
	//------------
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	//------------
	size_t VBO;
	glGenBuffers(1, &VBO);
	size_t VAO;
	glGenVertexArrays(1, &VAO);
	//------------
	glBindVertexArray(VAO);
	//------------
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//------------
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//------------
	size_t texture[2];
	glGenTextures(2, texture);
	string imgName[2] = {
		rootPath + "/data/textures/container.jpg",
		rootPath + "/data/textures/awesomeface.png"
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
	string prefix = rootPath + "/data/shaders/" + chapter + "/" + subchapter + "/";
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
	glm::mat4 view(1.0f);
	// 注意，我们将矩阵向我们要进行移动场景的反方向移动。
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
	//------------
	
 	auto registerInputOp = new RegisterInput(1, windowWidth, windowHeight, &projection, false);

	//-------------
	auto renderOp = new LambdaOp([&]() {
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//------------ 
		glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活纹理单元
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glActiveTexture(GL_TEXTURE1); // 在绑定纹理之前先激活纹理单元
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		//------------
		shader.Use();
		glBindVertexArray(VAO);
		//------------
		shader.SetMat4f("view", glm::value_ptr(view));
		shader.SetMat4f("projection", glm::value_ptr(projection));

		for(size_t i = 0; i < 10; i++){
			glm::mat4 model(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = (float)glfwGetTime() * 50.0f + 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.SetMat4f("model", glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
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
	EventManager::GetInstance()->Register(EventManager::KEYBOARD | GLFW_KEY_ESCAPE, closeWindowOp);

	// Polygon Mode
	//------------ GLFW_KEY_1
	auto polygonModeFillOp = new LambdaOp([]() {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	});
	EventManager::GetInstance()->Register(EventManager::KEYBOARD | GLFW_KEY_1, polygonModeFillOp);
	//------------ GLFW_KEY_2
	auto polygonModeLineOp = new LambdaOp([]() {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	});
	EventManager::GetInstance()->Register(EventManager::KEYBOARD | GLFW_KEY_2, polygonModeLineOp);

	// Texture Warp
	size_t info0 = GL_TEXTURE0 + textureID;
	//------------ GLFW_KEY_3
	auto textWarpReapeatOp = new InfoLambdaOp<size_t>(info0, []() {
		auto textWarpReapeatOp = *GStorage<InfoLambdaOp<size_t> *>::GetInstance()->GetPtr("textWarpReapeatOp");
		glBindTexture(GL_TEXTURE_2D, textWarpReapeatOp->GetInfo());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	});
	GStorage<InfoLambdaOp<size_t> *>::GetInstance()->Register("textWarpReapeatOp", textWarpReapeatOp);
	EventManager::GetInstance()->Register(EventManager::KEYBOARD | GLFW_KEY_3, textWarpReapeatOp);
	//------------ GLFW_KEY_4
	auto textWarpMirrorReapeatOp = new InfoLambdaOp<size_t>(info0, []() {
		auto textWarpMirrorReapeatOp = *GStorage<InfoLambdaOp<size_t> *>::GetInstance()->GetPtr("textWarpMirrorReapeatOp");
		glBindTexture(GL_TEXTURE_2D, textWarpMirrorReapeatOp->GetInfo());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	});
	GStorage<InfoLambdaOp<size_t> *>::GetInstance()->Register("textWarpMirrorReapeatOp", textWarpMirrorReapeatOp);
	EventManager::GetInstance()->Register(EventManager::KEYBOARD | GLFW_KEY_4, textWarpMirrorReapeatOp);
	//------------ GLFW_KEY_5
	auto textWarpClamp2EdgeOp = new InfoLambdaOp<size_t>(info0, []() {
		auto textWarpClamp2EdgeOp = *GStorage<InfoLambdaOp<size_t> *>::GetInstance()->GetPtr("textWarpClamp2EdgeOp");
		glBindTexture(GL_TEXTURE_2D, textWarpClamp2EdgeOp->GetInfo());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	});
	GStorage<InfoLambdaOp<size_t> *>::GetInstance()->Register("textWarpClamp2EdgeOp", textWarpClamp2EdgeOp);
	EventManager::GetInstance()->Register(EventManager::KEYBOARD | GLFW_KEY_5, textWarpClamp2EdgeOp);
	//------------ GLFW_KEY_6
	auto textWarpClamp2BodderOp = new InfoLambdaOp<size_t>(info0, []() {
		auto textWarpClamp2BodderOp = *GStorage<InfoLambdaOp<size_t> *>::GetInstance()->GetPtr("textWarpClamp2BodderOp");
		glBindTexture(GL_TEXTURE_2D, textWarpClamp2BodderOp->GetInfo());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	});
	GStorage<InfoLambdaOp<size_t> *>::GetInstance()->Register("textWarpClamp2BodderOp", textWarpClamp2BodderOp);
	EventManager::GetInstance()->Register(EventManager::KEYBOARD | GLFW_KEY_6, textWarpClamp2BodderOp);

	// Projection
	Info1 info1 = { projection, (float)windowWidth , (float)windowHeight };
	//------------ GLFW_KEY_7
	auto perspectiveOp = new InfoLambdaOp<Info1>(info1, []() {
		auto perspectiveOp = *GStorage<InfoLambdaOp<Info1> *>::GetInstance()->GetPtr("perspectiveOp");
		auto & info = perspectiveOp->GetInfo();
		(*info.projection) = glm::perspective(glm::radians(45.0f), info.width / info.height, 0.1f, 100.0f);
	});
	GStorage<InfoLambdaOp<Info1> *>::GetInstance()->Register("perspectiveOp", perspectiveOp);
	EventManager::GetInstance()->Register(EventManager::KEYBOARD | GLFW_KEY_7, perspectiveOp);
	//------------ GLFW_KEY_8
	auto orthoOp = new InfoLambdaOp<Info1>(info1, []() {
		auto orthoOp = *GStorage<InfoLambdaOp<Info1> *>::GetInstance()->GetPtr("orthoOp");
		auto & info = orthoOp->GetInfo();
		(*info.projection) = glm::ortho(-10.0f, 10.0f, -10 * info.height / info.width, 10 * info.height / info.width, 0.01f, 100.0f);
	});
	GStorage<InfoLambdaOp<Info1> *>::GetInstance()->Register("orthoOp", orthoOp);
	EventManager::GetInstance()->Register(EventManager::KEYBOARD | GLFW_KEY_8, orthoOp);

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

Config * DoConfig() {
	printf("Try to read config.out\n");
	string rootPath;
	Config * config = new Config;
	rootPath = string(ROOT_PATH);
	printf("[1] First Try.\n");
	config->Load(rootPath + "/config/config.out");
	if (!config->IsValid()) {
		rootPath = "../../..";
		printf("[2] Second Try.\n");
		config->Load(rootPath + "/config/config.out");
		if (!config->IsValid()) {
			rootPath = ".";
			printf("[3] Third Try.\n");
			config->Load(rootPath + "/config.out");
			if (!config->IsValid()) {
				printf(
					"ERROR : Three Try All Fail\n"
					"ERROR : RootPath is not valid.\n"
					"Please change config/config.out 's value of RootPath and\n"
					"run exe in correct place( original place or same palce with config.out ).\n");
				exit(1);
			}
		}
	}
	*config->GetStrPtr("RootPath") = rootPath;
	printf("config.out read success\nRootPath is %s\n", config->GetStrPtr("RootPath")->c_str());
	GStorage<Config *>::GetInstance()->Register(str_MainConfig, config);
	return config;
}