#include <ROOT_PATH.h>
#include <GLFW/Glfw.h>
#include <Shader.h>
#include <Utility/Image.h>
#include <Utility/InfoLambdaOp.h>

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
	string subchapter = "04_Texture";
	//------------
	size_t windowWidth = 1024, windowHeight = 768;
	string windowTitle = chapter + "/" + subchapter;
	Glfw::GetInstance()->Init(windowWidth, windowHeight, windowTitle.c_str());
	//------------
	float vertices[] = {
		//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // ����
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // ����
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // ����
	};
	size_t indices[] = { // ע��������0��ʼ! 
		0, 1, 3, // ��һ��������
		1, 2, 3  // �ڶ���������
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
		// Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		img[i].Load(imgName[i].c_str(), flip[i]);
		if (!img[i].IsValid()) {
			cout << "Failed to load texture[" << imgName[i] << "]\n";
			return 1;
		}
		/*
		@1 ����Ŀ��
		@2 �༶��Զ����ļ��� ��0 Ϊ��������)
		@3 �����ʽ
		@4 width
		@5 height
		@6 0 (��ʷ��������)
		@7 Դͼ��ʽ
		@8 Դͼ����
		@9 ͼ������
		*/
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img[i].GetWidth(), img[i].GetHeight(), 0, mode[i], GL_UNSIGNED_BYTE, img[i].GetData());
		// Ϊ��ǰ�󶨵������Զ�����������Ҫ�Ķ༶��Զ����
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
	
	auto registerInputOp = new RegisterInput(texture[1], false);
	
	//-------------
	auto renderOp = new LambdaOp([&]() {
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//------------ 
		glActiveTexture(GL_TEXTURE0); // �ڰ�����֮ǰ�ȼ�������Ԫ
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glActiveTexture(GL_TEXTURE1); // �ڰ�����֮ǰ�ȼ�������Ԫ
		glBindTexture(GL_TEXTURE_2D, texture[1]);
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
	//OpQueue opQueue(); <--- ����������Ϊ������һ������
	auto opQueue = new OpQueue;
	(*opQueue) << registerInputOp << renderOp << endOp;
	//------------
	Glfw::GetInstance()->Run(opQueue);
	//------------
	Glfw::GetInstance()->Terminate();
	return 0;
}

void RegisterInput::Run() {
	//Close Window
	auto closeWindowOp = new LambdaOp([]() {
		Glfw::GetInstance()->CloseWindow();
	});
	EventManager::GetInstance()->RegisterOp(GLFW_KEY_ESCAPE, closeWindowOp);

	//Polygon Mode
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

	//Texture Warp
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

	//------------

	cout << endl
		<< "* 1. Press '1' to set PolygonMode[FILL]" << endl
		<< "* 2. Press '2' to set PolygonMode[LINE]" << endl
		<< "* 3. Press '3' to set TEXTURE_WRAP[REPEAT]" << endl
		<< "* 4. Press '4' to set TEXTURE_WRAP[MIRRORED_REPEAT]" << endl
		<< "* 5. Press '5' to set TEXTURE_WRAP[CLAMP_TO_EDGE]" << endl
		<< "* 6. Press '6' to set TEXTURE_WRAP[CLAMP_TO_BORDER]" << endl
		<< "* 7. Press 'ESC' to close exe" << endl << endl;
}