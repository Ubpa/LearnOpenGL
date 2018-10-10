#include <Utility/Operation.h>

using namespace Ubpa;
using namespace std;

Operation::Operation(bool isHold): isHold(isHold){};
bool Operation::IsHold() { return isHold; }
void Operation::SetIsHold(bool isHold) { this->isHold = isHold; }
Operation::~Operation() {}

void Operation::Run() { this->operator()(); }


LambdaOperation::LambdaOperation(const std::function<void()> & operation, bool isHold = true)
	: Operation(isHold), operation(operation) { }

void LambdaOperation::operator()() { operation(); };

OperationQueue::OperationQueue(bool isHold)
	: Operation(isHold) { }


void OperationQueue::push(Operation & operation) {
	push(&operation);
}
OperationQueue & OperationQueue::operator<<(Operation & operation) {
	push(&operation);
	return *this;
}

void OperationQueue::operator()() {
	size_t n = size();
	for (size_t i = 0; i < n; i++) {
		front()->Run();
		if (front()->IsHold())
			push(front());
		pop();
	}
}
