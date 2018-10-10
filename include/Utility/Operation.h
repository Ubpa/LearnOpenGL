#ifndef _OPERATION_H_
#define _OPERATION_H_

#include<queue>
#include<functional>

namespace Ubpa{
	class Operation{
	public:
		Operation(bool isHold);
		bool IsHold();
		void Run();
		virtual void operator()() = 0;
		virtual ~Operation();
	protected:
		bool isHold;
	};

	class LambdaOperation : public Operation{
	public:
		LambdaOperation(std::function<void()> operation, bool isHold = false);
		void operator()();
	private:
		std::function<void()> operation;
	};

	class OperationQueue : public std::queue<Operation *>, public Operation {
	public:
		OperationQueue(bool isHold = false);
		void operator()();
	};
};
#endif//! _FILE_H_