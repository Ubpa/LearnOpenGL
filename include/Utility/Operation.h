#ifndef _OPERATION_H_
#define _OPERATION_H_

#include<functional>
#include<list>
#include<string>
#include<memory>
#include<map>
#include<Utility/Storage.h>

namespace Ubpa{
	template<typename T>
	using Ptr = std::shared_ptr<T>;
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
		//------------
		static Operation * GetFromStorage(const std::string & ID);
	protected:
		virtual ~Operation() { printf("Delete Operation\n"); };
		//------------
		bool isHold;
	private:
		Operation(const Operation&) = default;
		Operation& operator=(const Operation&) = default;
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
		std::function<void()> op;
	private:
		LambdaOp(const LambdaOp&) = default;
		LambdaOp& operator=(const LambdaOp&) = default;
	};
	
	//InfoLambdaOp
	template<typename T>
	class InfoLambdaOp : public LambdaOp {
	public:
		InfoLambdaOp(const std::string & ID, const T & info = T(), const std::function<void()> & op = []() {}, bool isHold = true);
		//------------
		void SetInfo(const T & info);
		T & GetInfo();
		//------------
		static InfoLambdaOp<T> * GetFromStorage(const std::string & ID);
	protected:
		virtual ~InfoLambdaOp();
		std::string ID;
		T info;
	private:
		InfoLambdaOp(const InfoLambdaOp&) = default;
		InfoLambdaOp& operator=(const InfoLambdaOp&) = default;
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
		OpQueue(const OpQueue &) = default;
		OpQueue& operator=(const OpQueue &) = default;
	};

	//------------

	template<typename T>
	InfoLambdaOp<T>::InfoLambdaOp(const std::string & ID, const T & info, const std::function<void()> & op, bool isHold)
		:info(info), ID(ID), LambdaOp(op, isHold){
		Storage<Operation *>::GetInstance()->Register(ID, this);
	}

	template<typename T>
	void InfoLambdaOp<T>::SetInfo(const T & info) {
		this->info = info;
	}

	template<typename T>
	T & InfoLambdaOp<T>::GetInfo() {
		return info;
	}

	template<typename T>
	InfoLambdaOp<T>::~InfoLambdaOp() {
		Storage<Operation *>::GetInstance()->Unregister(ID);
		printf("Delete InfoLambdaOp\n");
	};
	template<typename T>
	InfoLambdaOp<T> * InfoLambdaOp<T>::GetFromStorage(const std::string & ID) {
		auto target = Operation::GetFromStorage(ID);
		return dynamic_cast<InfoLambdaOp<T> *>(target);
	}
};
#endif//! _FILE_H_