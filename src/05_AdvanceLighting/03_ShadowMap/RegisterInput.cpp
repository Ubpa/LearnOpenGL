#include "RegisterInput.h"

#include "Defines.h"

#include <GLFW/Glfw.h>

#include <Utility/GStorage.h>
#include <Utility/LambdaOp.h>

#include <LOGL/Camera.h>
#include <LOGL/Shader.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

using namespace Ubpa;
using namespace LOGL;
using namespace Define;
using namespace std;

void RegisterInput::Run() {
	RegisterMouse();
	RegisterKey();
	PrintInfo();
}

void RegisterInput::RegisterMouse() {
	EventManager::GetInstance()->Register(EventManager::MOUSE_MOUVE, [](){
		auto mainCamera = *GStorage<Camera *>::GetInstance()->GetPtr(str_MainCamera);
		auto xOffset = **GStorage<float *>::GetInstance()->GetPtr("mousePos_XOffset");
		auto yOffset = **GStorage<float *>::GetInstance()->GetPtr("mousePos_YOffset");
		mainCamera->ProcessMouseMovement(xOffset, yOffset);
	});
	EventManager::GetInstance()->Register(EventManager::MOUSE_SCROLL, []() {
		auto mainCamera = *GStorage<Camera *>::GetInstance()->GetPtr(str_MainCamera);
		auto mouseScroll_YOffset = **GStorage<float *>::GetInstance()->GetPtr("mouseScroll_YOffset");
		mainCamera->ProcessMouseScroll(mouseScroll_YOffset);
	});
}

void RegisterInput::RegisterKey() {
	// Close Window
	auto closeWindowOp = new LambdaOp();
	EventManager::GetInstance()->Register(EventManager::KEYBOARD_PRESS | GLFW_KEY_ESCAPE,
		[]() { Glfw::GetInstance()->CloseWindow(); });
	
	// Polygon Mode
	//------------ GLFW_KEY_1
	EventManager::GetInstance()->Register(EventManager::KEYBOARD_PRESS | GLFW_KEY_U,
		[]() { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); });
	//------------ GLFW_KEY_2
	EventManager::GetInstance()->Register(EventManager::KEYBOARD_PRESS | GLFW_KEY_I,
		[]() { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); });

	// Projection
	//------------ GLFW_KEY_3
	EventManager::GetInstance()->Register(EventManager::KEYBOARD_PRESS | GLFW_KEY_O, 
		[]() {
		auto mainCamera = *GStorage<Camera *>::GetInstance()->GetPtr(str_MainCamera);
		mainCamera->SetPerspective();
	});
	//------------ GLFW_KEY_4
	EventManager::GetInstance()->Register(EventManager::KEYBOARD_PRESS | GLFW_KEY_P, 
		[]() {
		auto mainCamera = *GStorage<Camera *>::GetInstance()->GetPtr(str_MainCamera);
		mainCamera->SetOrtho();
	});


	// Move
	size_t moveKey[] = { GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_Q, GLFW_KEY_E };
	size_t arrowKey[] = { GLFW_KEY_UP, GLFW_KEY_DOWN, GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_PAGE_UP, GLFW_KEY_PAGE_DOWN };
	for (size_t i = 0; i < 6; i++) {
		LambdaOp * op = new LambdaOp([=]() {
			auto mainCamera = *GStorage<Camera *>::GetInstance()->GetPtr(str_MainCamera);
			auto deltaTime = **GStorage<float *>::GetInstance()->GetPtr(str_DeltaTime);
			mainCamera->ProcessKeyboard(Camera::ENUM_Movement(Camera::MOVE_FORWARD + i), deltaTime);
		});

		EventManager::GetInstance()->Register(EventManager::KEYBOARD | moveKey[i],
			op);
		EventManager::GetInstance()->Register(EventManager::KEYBOARD | arrowKey[i],
			op);
	}
	/*
	EventManager::GetInstance()->Register(EventManager::KEYBOARD_PRESS | GLFW_KEY_G, [=]() {
		auto pGammaEnable = *GStorage<bool *>::GetInstance()->GetPtr(str_GammaEnable);
		*pGammaEnable = !*pGammaEnable;
		printf("INFO: Switch to %s\n", *pGammaEnable ? "Gamma Enable" : "Gamma Disable");
	});
	*/
}

void RegisterInput::PrintInfo() {
	cout << endl
		<< "* 1. Press 'U' to set PolygonMode[FILL]" << endl
		<< "* 2. Press 'I' to set PolygonMode[LINE]" << endl
		<< "* 3. Press 'O' to set Projection[perspective]" << endl
		<< "* 4. Press 'P' to set Projection[ortho]" << endl
		//<< "* 5. Press 'G' to switch Gamma Correction mode" << endl
		<< "* 5. Key [WSADQE] and [left,right,up,down,pageup,pagedown] to control the position."
		<< "* 6. Press 'ESC' to close exe" << endl << endl;
}