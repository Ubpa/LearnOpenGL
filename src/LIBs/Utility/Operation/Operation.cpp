#include <Utility/Operation.h>

using namespace Ubpa;
using namespace std;
//-------------
Operation::Operation(bool isHold): isHold(isHold){};

bool Operation::IsHold() { return isHold; }

void Operation::SetIsHold(bool isHold) { this->isHold = isHold; }

Operation::~Operation() {}

void Operation::Run() { this->operator()(); }
//------------
LambdaOp::LambdaOp(const std::function<void()> & operation, bool isHold)
	: Operation(isHold), operation(operation) { }

void LambdaOp::operator()() { operation(); };

Operation * LambdaOp::NewCopy() const {
	return new LambdaOp(operation, isHold);
}
//-------------
OpQueue::OpQueue(bool isHold)
	: Operation(isHold) { }

OpQueue::~OpQueue() {
	while (!opDq.empty()) {
		delete opDq.front();
		opDq.pop_front();
	}
}

Operation * OpQueue::NewCopy() const {
	OpQueue * newOpQueue = new OpQueue(isHold);
	for (auto it = opDq.begin(); it != opDq.end(); ++it)
		newOpQueue->Push(**it);
	return newOpQueue;
}

void OpQueue::Push(const Operation & op) {
	opDq.push_back(op.NewCopy());
}

OpQueue & OpQueue::operator<<(const Operation & operation) {
	Push(operation);
	return *this;
}

void OpQueue::operator()() {
	size_t n = opDq.size();
	for (size_t i = 0; i < n; i++) {
		opDq.front()->Run();
		if (opDq.front()->IsHold())
			opDq.push_back(opDq.front());
		else
			delete opDq.front();
		opDq.pop_front();
	}
}

size_t OpQueue::Size() {
	return opDq.size();
}
