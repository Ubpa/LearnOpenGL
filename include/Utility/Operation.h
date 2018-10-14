#ifndef _OPERATION_H_
#define _OPERATION_H_

#include <string>

namespace Ubpa{
	template<typename T>
	using Ptr = std::shared_ptr<T>;

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
		virtual ~Operation();
		//------------
		bool isHold;
	private:
		Operation(const Operation&) = default;
		Operation& operator=(const Operation&) = default;
	};
};
#endif//! _FILE_H_