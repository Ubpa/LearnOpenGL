#include <Utility/Operation.h>

using namespace Ubpa;
using namespace std;

Operation::Operation(bool isHold): isHold(isHold){};
bool Operation::IsHold() { return isHold; }

Operation::~Operation() {}

void Operation::Run() { this->operator()(); }

LambdaOperation::LambdaOperation(function<void()> operation, bool isHold)
	: Operation(isHold), operation(operation) { }

void LambdaOperation::operator()() { operation(); };

OperationQueue::OperationQueue(bool isHold)
	: Operation(isHold) { }

void OperationQueue::operator()(){
	size_t n = size();
	for (size_t i = 0; i < n; i++) {
		if (front() != NULL) {
			front()->Run();
			if (front()->IsHold())
				push(front());
		}
		pop();
	}
}
