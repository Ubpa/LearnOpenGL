#include "RegisterInput.h"

#include <GLFW/Glfw.h>
#include <Utility/InfoLambdaOp.h>
#include <Camera.h>

using namespace Ubpa;
using namespace LOGL;
using namespace std;

void RegisterInput::Run() {
	RegisterMouse();
	RegisterKey();
	PrintInfo();
}

void RegisterInput::RegisterMouse() {
	EventManager::GetInstance()->RegisterOp(EventManager::MOUSE_MOUVE, [](){
		auto mainCamera = *Storage<Camera *>::GetInstance()->Get("mainCamera");
		auto xOffset = **Storage<float *>::GetInstance()->Get("mousePos_XOffset");
		auto yOffset = **Storage<float *>::GetInstance()->Get("mousePos_YOffset");
		mainCamera->ProcessMouseMovement(xOffset, yOffset);
	});
	EventManager::GetInstance()->RegisterOp(EventManager::MOUSE_SCROLL, []() {
		auto mainCamera = *Storage<Camera *>::GetInstance()->Get("mainCamera");
		auto mouseScroll_YOffset = **Storage<float *>::GetInstance()->Get("mouseScroll_YOffset");
		mainCamera->ProcessMouseScroll(mouseScroll_YOffset);
	});
}

void RegisterInput::RegisterKey() {
	// Close Window
	auto closeWindowOp = new LambdaOp();
	EventManager::GetInstance()->RegisterOp(EventManager::KEYBOARD_PRESS | GLFW_KEY_ESCAPE,
		[]() { Glfw::GetInstance()->CloseWindow(); });


	// Polygon Mode
	//------------ GLFW_KEY_1
	EventManager::GetInstance()->RegisterOp(EventManager::KEYBOARD_PRESS | GLFW_KEY_1,
		[]() { glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); });
	//------------ GLFW_KEY_2
	EventManager::GetInstance()->RegisterOp(EventManager::KEYBOARD_PRESS | GLFW_KEY_2,
		[]() { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); });


	// Texture Warp
	size_t info0 = GL_TEXTURE0 + textureID;
	//------------ GLFW_KEY_3
	auto textWarpReapeatOp = new InfoLambdaOp<size_t>("textWarpReapeatOp", info0, []() {
		auto textWarpReapeatOp = InfoLambdaOp<size_t>::GetFromStorage("textWarpReapeatOp");
		glBindTexture(GL_TEXTURE_2D, textWarpReapeatOp->GetInfo());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	});
	EventManager::GetInstance()->RegisterOp(EventManager::KEYBOARD_PRESS | GLFW_KEY_3, textWarpReapeatOp);
	//------------ GLFW_KEY_4
	auto textWarpMirrorReapeatOp = new InfoLambdaOp<size_t>("textWarpMirrorReapeatOp", info0, []() {
		auto textWarpMirrorReapeatOp = InfoLambdaOp<size_t>::GetFromStorage("textWarpMirrorReapeatOp");
		glBindTexture(GL_TEXTURE_2D, textWarpMirrorReapeatOp->GetInfo());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	});
	EventManager::GetInstance()->RegisterOp(EventManager::KEYBOARD_PRESS | GLFW_KEY_4, textWarpMirrorReapeatOp);
	//------------ GLFW_KEY_5
	auto textWarpClamp2EdgeOp = new InfoLambdaOp<size_t>("textWarpClamp2EdgeOp", info0, []() {
		auto textWarpClamp2EdgeOp = InfoLambdaOp<size_t>::GetFromStorage("textWarpClamp2EdgeOp");
		glBindTexture(GL_TEXTURE_2D, textWarpClamp2EdgeOp->GetInfo());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	});
	EventManager::GetInstance()->RegisterOp(EventManager::KEYBOARD_PRESS | GLFW_KEY_5, textWarpClamp2EdgeOp);
	//------------ GLFW_KEY_6
	auto textWarpClamp2BodderOp = new InfoLambdaOp<size_t>("textWarpClamp2BodderOp", info0, []() {
		auto textWarpClamp2BodderOp = InfoLambdaOp<size_t>::GetFromStorage("textWarpClamp2BodderOp");
		glBindTexture(GL_TEXTURE_2D, textWarpClamp2BodderOp->GetInfo());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	});
	EventManager::GetInstance()->RegisterOp(EventManager::KEYBOARD_PRESS | GLFW_KEY_6, textWarpClamp2BodderOp);


	// Projection
	//------------ GLFW_KEY_7
	EventManager::GetInstance()->RegisterOp(EventManager::KEYBOARD_PRESS | GLFW_KEY_7, 
		[]() {
		auto mainCamera = *Storage<Camera *>::GetInstance()->Get("mainCamera");
		mainCamera->SetPerspective();
	});
	//------------ GLFW_KEY_8
	EventManager::GetInstance()->RegisterOp(EventManager::KEYBOARD_PRESS | GLFW_KEY_8, 
		[]() {
		auto mainCamera = *Storage<Camera *>::GetInstance()->Get("mainCamera");
		mainCamera->SetOrtho();
	});


	// Move
	//------------ GLFW_KEY_W
	EventManager::GetInstance()->RegisterOp(EventManager::KEYBOARD | GLFW_KEY_W,
		[]() {
		auto mainCamera = *Storage<Camera *>::GetInstance()->Get("mainCamera");
		auto deltaTime = **Storage<float *>::GetInstance()->Get("deltaTime");
		mainCamera->ProcessKeyboard(Camera::MOVE_FORWARD, deltaTime);
	});
	//------------ GLFW_KEY_S
	EventManager::GetInstance()->RegisterOp(EventManager::KEYBOARD | GLFW_KEY_S,
		[]() {
		auto mainCamera = *Storage<Camera *>::GetInstance()->Get("mainCamera");
		auto deltaTime = **Storage<float *>::GetInstance()->Get("deltaTime");
		mainCamera->ProcessKeyboard(Camera::MOVE_BACKWARD, deltaTime);
	});
	//------------ GLFW_KEY_A
	EventManager::GetInstance()->RegisterOp(EventManager::KEYBOARD | GLFW_KEY_A,
		[]() {
		auto mainCamera = *Storage<Camera *>::GetInstance()->Get("mainCamera");
		auto deltaTime = **Storage<float *>::GetInstance()->Get("deltaTime");
		mainCamera->ProcessKeyboard(Camera::MOVE_LEFT, deltaTime);
	});
	//------------ GLFW_KEY_D
	EventManager::GetInstance()->RegisterOp(EventManager::KEYBOARD | GLFW_KEY_D,
		[]() {
		auto mainCamera = *Storage<Camera *>::GetInstance()->Get("mainCamera");
		auto deltaTime = **Storage<float *>::GetInstance()->Get("deltaTime");
		mainCamera->ProcessKeyboard(Camera::MOVE_RIGHT, deltaTime);
	});
	//------------ GLFW_KEY_Q
	EventManager::GetInstance()->RegisterOp(EventManager::KEYBOARD | GLFW_KEY_Q,
		[]() {
		auto mainCamera = *Storage<Camera *>::GetInstance()->Get("mainCamera");
		auto deltaTime = **Storage<float *>::GetInstance()->Get("deltaTime");
		mainCamera->ProcessKeyboard(Camera::MOVE_UP, deltaTime);
	});
	//------------ GLFW_KEY_E
	EventManager::GetInstance()->RegisterOp(EventManager::KEYBOARD | GLFW_KEY_E,
		[]() {
		auto mainCamera = *Storage<Camera *>::GetInstance()->Get("mainCamera");
		auto deltaTime = **Storage<float *>::GetInstance()->Get("deltaTime");
		mainCamera->ProcessKeyboard(Camera::MOVE_DOWN, deltaTime);
	});

}

void RegisterInput::PrintInfo() {
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