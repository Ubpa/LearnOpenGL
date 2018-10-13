#include <Utility/Operation.h>
#include <vector>

using namespace Ubpa;
using namespace std;
//-------------
Operation::Operation(bool isHold): isHold(isHold){};

bool Operation::IsHold() { return isHold; }

void Operation::SetIsHold(bool isHold) { this->isHold = isHold; }

void Operation::operator()() { Run(); }

//------------

LambdaOp::LambdaOp(const std::function<void()> & op, bool isHold)
	:op(op) {
	this->isHold = isHold;
}

void LambdaOp::SetOp(const std::function<void()> & op) {
	this->op = op;
}

void LambdaOp::Run() {
	op();
}

//-------------

OpQueue::OpQueue(bool isHold) 
	: Operation(true) { }

OpQueue & OpQueue::operator<<(Ptr<Operation> & op) {
	opList.push_back(op);
	return *this;
}

OpQueue & OpQueue::operator<<(Operation * op) {
	opList.push_back(Ptr<Operation>(op, Operation::Delete));
	return *this;
}

void OpQueue::Push(Ptr<Operation> & op) {
	opList.push_back(op);
}

void OpQueue::Push(Operation * op) {
	opList.push_back(Ptr<Operation>(op, Operation::Delete));
}
size_t OpQueue::Size() const {
	return opList.size();
}

void OpQueue::Run() {
	vector< list<Ptr<Operation>>::const_iterator > removeIt;
	for (auto it = opList.cbegin(); it != opList.cend(); ++it) {
		(*it)->Run();
		if (!(*it)->IsHold())
			removeIt.push_back(it);
	}
	for (int i = removeIt.size() - 1; i >= 0; i--)
		opList.erase(removeIt[i]);
}