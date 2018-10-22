#ifndef _OPERATION_H_
#define _OPERATION_H_

#include <string>

namespace Ubpa{
	template<typename T>
	using Ptr = std::shared_ptr<T>;

	class Operation{
	public:
		template <typename T>
		static Ptr<T> ToPtr(T * op) {
			return Ptr<T>(op, ProtectDelete);
		}
		Operation(bool isHold = true);
		//------------
		bool IsHold();
		void SetIsHold(bool isHold);
		void operator()();
		//------------
		virtual void Run() = 0;
	protected:
		static void ProtectDelete(Operation * op);
		virtual ~Operation();
		//------------
		bool isHold;
	private:
		Operation(const Operation&) = default;
		Operation& operator=(const Operation&) = default;
	};
};
#endif//! _FILE_H_