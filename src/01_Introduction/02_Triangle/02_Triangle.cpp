#include <ROOT_PATH.h>
#include <GLFW/Glfw.h>
#include <LOGL/Shader.h>

#include <Utility/LambdaOp.h>

using namespace LOGL;
using namespace Ubpa;
using namespace std;

void registerInput();


#include <Utility/Config.h>
#include <Utility/GStorage.h>
const string str_MainConfig = "MainConfig";

Config * DoConfig();

int main(int argc, char ** argv) {
	Config * config = DoConfig();
	string rootPath = *config->GetStrPtr("RootPath");
	size_t width = 800, height = 600;
	string chapter = "01_Introduction";
	string subchapter = "02_Triangle";
	//------------
	size_t windowWidth = 1024, windowHeight = 768;
	string windowTitle = chapter + "/" + subchapter;
	Glfw::GetInstance()->Init(windowWidth, windowHeight, windowTitle.c_str());
	//------------
	float vertices0[] = {
		0.5f, 0.5f, 0.0f,   // ���Ͻ�
		0.5f, -0.5f, 0.0f,  // ���½�
		-0.5f, -0.5f, 0.0f, // ���½�
		-0.5f, 0.5f, 0.0f,   // ���Ͻ�
	};
	float vertices1[] = {
		-0.5f, -0.5f, 0.0f, // ���½�
		-0.5f, 0.5f, 0.0f,   // ���Ͻ�
		-1.0f, 0.0f, 0.0f   // ��
	};
	size_t indices[] = { // ע��������0��ʼ! 
		0, 1, 3, // ��һ��������
		1, 2, 3  // �ڶ���������
	};
	//------------ 0. ���� VBO , VAO �� EBO
	size_t VBO[2];
	glGenBuffers(2, VBO);
	size_t VAO[2];
	glGenVertexArrays(2, VAO);
	size_t EBO;
	glGenBuffers(1, &EBO);
	//------------ 1. �� VAO
	glBindVertexArray(VAO[0]);
	//------------ 2. �� VBO, �����������͵� VBO ��
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	/*
	* GL_STATIC_DRAW �����ݲ���򼸺�����ı䡣
	* GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶࡣ
	* GL_STREAM_DRAW ������ÿ�λ���ʱ����ı䡣
	*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices0), vertices0, GL_STATIC_DRAW);
	//------------ 3. �� EBO, �����������͵� EBO ��
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//------------ 4. �󶨶�������ָ��
	/*
	* @1 Location : 0
	* @2 Attrib size : 3
	* @3 Type : float
	* @4 Normalize : false
	*     - signed type -> (-1, 1)
	*     - unsigned type -> (0, 1)
	* @5 Stride : 3 * sizeof(float)
	* @6 Offset : 0
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);
	//------------ 5. �ظ�1, 2 �� 4, ���趨һ�� VAO
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);
	//------------ 5. ������ɫ��
	string prefix = rootPath + "/data/shaders/01_Introduction/02_Triangle/";
	string vsF = prefix + "02_Triangle.vs";
	string fsRedF = prefix + "02_Triangle_Red.fs";
	string fsBlueF = prefix + "02_Triangle_Blue.fs";
	Shader shaderRed(vsF.c_str(), fsRedF.c_str());
	Shader shaderBlue(vsF.c_str(), fsBlueF.c_str());
	if (!shaderRed.IsValid() || !shaderBlue.IsValid()) {
		cout << "Shader is not Valid\n";
		return 1;
	}
	//------------ 6. ������Ⱦѭ��
	auto initOp = new LambdaOp(registerInput, false);

	auto renderOp = new LambdaOp ([&]() {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//------------ Red
		glBindVertexArray(VAO[0]);
		shaderRed.Use();
		/*
		* @1 ����ģʽ
		* @2 ���ƵĶ������
		* @3 ��������
		* @4 ƫ����
		*/
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
		//------------ Blue
		glBindVertexArray(VAO[1]);
		shaderBlue.Use();
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}, true);

	auto endOp = new LambdaOp ([]() {
		glfwSwapBuffers(Glfw::GetInstance()->GetWindow());
		glfwPollEvents();
	});

	//OpQueue opQueue(); <--- ����������Ϊ������һ������
	auto opQueue = new OpQueue;
	(*opQueue) << initOp << renderOp << endOp;
	//------------
	Glfw::GetInstance()->Run(opQueue);
	//------------
	Glfw::GetInstance()->Terminate();
	return 0;
}

void registerInput() {
	EventManager::GetInstance()->Register(EventManager::KEYBOARD | GLFW_KEY_ESCAPE, []() {
		if (Glfw::GetInstance()->GetKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
			Glfw::GetInstance()->CloseWindow();
	});
	EventManager::GetInstance()->Register(EventManager::KEYBOARD | GLFW_KEY_1, []() {
		if (Glfw::GetInstance()->GetKey(GLFW_KEY_1) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	});
	EventManager::GetInstance()->Register(EventManager::KEYBOARD | GLFW_KEY_2, []() {
		if (Glfw::GetInstance()->GetKey(GLFW_KEY_2) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	});

	cout << endl
		<< "* 1. Press '1' to set PolygonMode[FILL]" << endl
		<< "* 2. Press '2' to set PolygonMode[LINE]" << endl
		<< "* 3. Press 'ESC' to close exe" << endl << endl;
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