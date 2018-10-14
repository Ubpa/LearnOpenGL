#ifndef _REGISTER_INPUT_H_
#define _REGISTER_INPUT_H_

#include <Utility/Operation.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class RegisterInput : public Ubpa::Operation {
public:
	RegisterInput(size_t textureID, bool isHold = true)
		:Operation(isHold), textureID(textureID){
	}
	void Run();
private:
	void RegisterMouse();
	void RegisterKey();
	void PrintInfo();
	//------------
	size_t textureID;
};

#endif // !_REGISTER_INPUT_H_
