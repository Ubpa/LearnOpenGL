#include <Utility/Operation.h>

using namespace Ubpa;

Operation::Operation(bool isHold): isHold(isHold){};
bool Operation::IsHold() { return isHold; }

Operation::~Operation(){};

void OperationVec::operator()(){
	for(size_t i = size()-1; i>=0; i--){
		at(i)();
		if (!at(i).IsHold())
			erase(begin() + i);
	}
}