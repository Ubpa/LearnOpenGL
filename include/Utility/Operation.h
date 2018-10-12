#ifndef _OPERATION_H_
#define _OPERATION_H_

#include<deque>
#include<functional>
#include<list>

namespace Ubpa{
	class Operation{
	public:
		Operation(bool isHold = true);
		bool IsHold();
		void SetIsHold(bool isHold);
		void Run();
		virtual Operation * NewCopy() const = 0;
		virtual void operator()() = 0;
		virtual ~Operation();
	protected:
		bool isHold;
	};

	class LambdaOp : public Operation{
	public:
		LambdaOp(const std::function<void()> & operation, bool isHold = true);
		void operator()();
		virtual Operation * NewCopy() const;
	private:
		std::function<void()> operation;
	};

	class OpQueue : public Operation {
	public:
		OpQueue(bool isHold = true);
		void Push(const Operation & op);
		OpQueue & operator<<(const Operation & operation);
		void operator()();
		virtual Operation * NewCopy() const;
		virtual ~OpQueue();
		size_t Size();
	private:
		OpQueue(const OpQueue&);
		OpQueue& operator=(const OpQueue&);
		//-------------
		std::deque<Operation *> opDq;
	};
};
#endif//! _FILE_H_