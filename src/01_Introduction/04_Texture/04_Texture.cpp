#include <ROOT_PATH.h>

#include <GLFW/Glfw.h>

#include <LOGL/Shader.h>
#include <LOGL/Texture.h>

#include <Utility/Image.h>
#include <Utility/InfoLambdaOp.h>
#include <Utility/GStorage.h>

#include <iostream>

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

#include <Utility/Config.h>
#include <Utility/GStorage.h>
const string str_MainConfig = "MainConfig";

Config * DoConfig();

int main(int argc, char ** argv) {
	Config * config = DoConfig();
	string rootPath = *config->GetStrPtr("RootPath");
	string chapter = "01_Introduction";
	string subchapter = "04_Texture";
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
	//------------ 纹理
	string imgPath[3] = {
		rootPath + "/data/textures/container.jpg",
		rootPath + "/data/textures/awesomeface.png",
		rootPath + "/data/textures/awesomeface.png",
	};
	bool flip[3] = { false, true, false };

	Texture texture0;
	if (!texture0.Load(imgPath[0], flip[0])) {
		printf("ERROR: Load texture [%s] fail.\n", imgPath[0].c_str());
		return 1;
	}

	Texture texture1(Texture::ENUM_TYPE_2D_DYNAMIC);
	Image texture1Imgs[3];
	texture1Imgs[0].Load(imgPath[1].c_str(), flip[1]);
	texture1Imgs[1].Load(imgPath[2].c_str(), flip[2]);
	const size_t imgW = 256, imgH = 192, imgC = 4;
	texture1Imgs[2].GenBuffer(imgW, imgH, imgC);
	for (size_t i = 0; i < imgW; i++) {
		for (size_t j = 0; j < imgH; j++) {
			float r = i / (float)imgW;
			float g = j / (float)imgH;
			float b = 2 / (1 / r + 1 / g);
			float a = (r + g + b) / 3;
			texture1Imgs[2].SetPixel(i, j, Image::Pixel<float>(r, g, b, a));
		}
	}
	texture1.SetImg(texture1Imgs[0]);
	
	//------------
	string prefix = rootPath + "/data/shaders/" + chapter + "/" + subchapter + "/";
	string vsF = prefix + subchapter + ".vs";
	string fsF = prefix + subchapter + ".fs";
	Shader shader(vsF.c_str(), fsF.c_str());
	if (!shader.IsValid()) {
		std::cout << "Shader is not Valid\n";
		return 1;
	}
	//------------
	shader.Use();
	shader.SetInt("texture0", 0);
	shader.SetInt("texture1", 1);
	//------------
	
	auto registerInputOp = new RegisterInput(texture1.GetID(), false);
	
	//------------- 时间
	float deltaTime = 0.0f; // 当前帧与上一帧的时间差
	GStorage<float *>::GetInstance()->Register("deltaTime", &deltaTime);
	float lastFrame = 0.0f; // 上一帧的时间
	auto timeUpdate = new LambdaOp([&]() {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	});

	auto updateTex1Op = new LambdaOp([&]() {
		static int second = 0;
		if (lastFrame - second < 1)
			return;

		second++;
		texture1.SetImg(texture1Imgs[second % 3]);
	});

	auto updateOp = new OpQueue;
	(*updateOp) << timeUpdate << updateTex1Op;

	//-------------
	auto renderOp = new LambdaOp([&]() {
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//------------ 
		glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活纹理单元
		glBindTexture(GL_TEXTURE_2D, texture0.GetID());
		glActiveTexture(GL_TEXTURE1); // 在绑定纹理之前先激活纹理单元
		glBindTexture(GL_TEXTURE_2D, texture1.GetID());
		//------------
		shader.Use();
		glBindVertexArray(VAO);
		float greenValue = (sinf(4.0f*glfwGetTime()) / 2.0f) + 0.5f;
		shader.SetVec4f("ourColor", 0.0f, greenValue, 0.0f, 1.0f);
		shader.SetVec3f("offset", sinf(4.0f*glfwGetTime()) * 0.5f, 0, 0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
	});
	//-------------
	auto endOp = new LambdaOp ([]() {
		glfwSwapBuffers(Glfw::GetInstance()->GetWindow());
		glfwPollEvents();
	});
	//-------------
	//OpQueue opQueue(); <--- 编译器会以为声明了一个函数
	auto opQueue = new OpQueue;
	(*opQueue) << registerInputOp << updateOp << renderOp << endOp;
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
	size_t info0 = textureID;
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

	//------------

	std::cout << endl
		<< "* 1. Press '1' to set PolygonMode[FILL]" << endl
		<< "* 2. Press '2' to set PolygonMode[LINE]" << endl
		<< "* 3. Press '3' to set TEXTURE_WRAP[REPEAT]" << endl
		<< "* 4. Press '4' to set TEXTURE_WRAP[MIRRORED_REPEAT]" << endl
		<< "* 5. Press '5' to set TEXTURE_WRAP[CLAMP_TO_EDGE]" << endl
		<< "* 6. Press '6' to set TEXTURE_WRAP[CLAMP_TO_BORDER]" << endl
		<< "* 7. Press 'ESC' to close exe" << endl << endl;
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