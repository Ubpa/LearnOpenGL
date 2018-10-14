#include <Utility/Operation.h>

using namespace Ubpa;
using namespace std;

Operation::Operation(bool isHold): isHold(isHold) { };
Operation::~Operation() { printf("Delete Operation\n"); };

bool Operation::IsHold() { return isHold; }

void Operation::SetIsHold(bool isHold) { this->isHold = isHold; }

void Operation::operator()() { Run(); }