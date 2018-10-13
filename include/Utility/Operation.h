#ifndef _OPERATION_H_
#define _OPERATION_H_

#include<functional>
#include<list>
#include<string>
#include<memory>

namespace Ubpa{
	template<typename T>
	using Ptr = std::shared_ptr<T>;
	static int pad = 0;
	//操作
	class Operation{
	public:
		static void Delete(Operation * op) { delete op; }
		Operation(bool isHold = true);
		//------------
		bool IsHold();
		void SetIsHold(bool isHold);
		void operator()();
		//------------
		virtual void Run() = 0;
	protected:
		virtual ~Operation() { printf("Delete Operation\n"); };
		//------------
		bool isHold;
	private:
		Operation(const Operation&);
		Operation& operator=(const Operation&);
	};

	//Lambda操作
	class LambdaOp : public Operation{
	public:
		LambdaOp(const std::function<void()> & op = []() {}, bool isHold = true);
		void SetOp(const std::function<void()> & op);
		//------------
		virtual void Run();
	protected:
		virtual ~LambdaOp() { printf("Delete LambdaOp\n"); };
	private:
		std::function<void()> op;
		//------------
		LambdaOp(const LambdaOp&);
		LambdaOp& operator=(const LambdaOp&);
	};

	//操作队列
	class OpQueue : public Operation {
	public:
		OpQueue(bool isHold = true);
		//------------
		OpQueue & operator<<(Ptr<Operation> & operation);
		OpQueue & operator<<(Operation * operation);
		void Push(Ptr<Operation> & op);
		void Push(Operation * op);
		size_t Size() const;
		//------------
		virtual void Run();
	protected:
		virtual ~OpQueue() { printf("Delete OpQueue\n"); };
		//------------
		std::list< Ptr<Operation> > opList;
	private:
		OpQueue(const OpQueue &);
		OpQueue& operator=(const OpQueue &);
	};
};
#endif//! _FILE_H_