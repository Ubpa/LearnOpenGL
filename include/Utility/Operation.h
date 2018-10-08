#ifndef _OPERATION_H_
#define _OPERATION_H_

#include<vector>

namespace Ubpa{
	class Operation{
	public:
		Operation(bool isHold);
		bool IsHold();
		virtual void operator()() = 0;
		virtual ~Operation();
	private:
		bool isHold;
	};

	class OperationVec : public std::vector<Operation>{
		void operator()();
	};
};
#endif//! _FILE_H_