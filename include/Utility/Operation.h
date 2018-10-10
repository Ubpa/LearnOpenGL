#ifndef _OPERATION_H_
#define _OPERATION_H_

#include<queue>
#include<functional>

namespace Ubpa{
	class Operation{
	public:
		Operation(bool isHold = true);
		bool IsHold();
		void SetIsHold(bool isHold);
		void Run();
		virtual void operator()() = 0;
		virtual ~Operation();
	protected:
		bool isHold;
	};

	class LambdaOperation : public Operation{
	public:
		LambdaOperation(const std::function<void()> & operation, bool isHold = true);
		void operator()();
	private:
		std::function<void()> operation;
	};

	class OperationQueue : public std::queue<Operation *>, public Operation {
	public:
		OperationQueue(bool isHold = true);
		void push(Operation &);
		OperationQueue & operator<<(Operation & operation);
		void operator()();
	private:
		using std::queue<Operation *>::push;
	};
};
#endif//! _FILE_H_