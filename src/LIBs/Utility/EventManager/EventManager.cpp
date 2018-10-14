#include <Utility/EventManager.h>

#include <Utility/LambdaOp.h>

using namespace Ubpa;

EventManager::EventManager() {}

EventManager * EventManager::instance = new EventManager();

EventManager * EventManager::GetInstance() {
	return instance;
}

//------------

void EventManager::RegisterOp(size_t event, Ptr<Operation> & op) {
	if (directory.find(event) == directory.end())
		directory[event] = Ptr<OpQueue>(new OpQueue, Operation::Delete);
	directory[event]->Push(op);
}

void EventManager::RegisterOp(size_t event, Operation * op) {
	RegisterOp(event, Ptr<Operation>(op, Operation::Delete));
}

void EventManager::RegisterOp(size_t event, const std::function<void ()> & op) {
	RegisterOp(event, new LambdaOp(op));
}

void EventManager::Response(size_t event) {
	auto it = directory.find(event);
	if (it != directory.end())
		it->second->Run();
}