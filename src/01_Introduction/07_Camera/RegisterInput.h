#ifndef _REGISTER_INPUT_H_
#define _REGISTER_INPUT_H_

#include <Utility/Operation.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class RegisterInput : public Ubpa::Operation {
public:
	RegisterInput(bool isHold = true)
		:Operation(isHold) { }
	void Run();
private:
	void RegisterMouse();
	void RegisterKey();
	void PrintInfo();
};

#endif // !_REGISTER_INPUT_H_
