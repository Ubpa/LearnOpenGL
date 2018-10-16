#ifndef _INFO_LAMBDA_OP_H_
#define _INFO_LAMBDA_OP_H_

#include <Utility/LambdaOp.h>
#include <functional>

namespace Ubpa{
	template<typename T>
	class InfoLambdaOp : public LambdaOp {
	public:
		InfoLambdaOp(const std::string & ID, const T & info = T(), const std::function<void()> & op = []() {}, bool isHold = true);
		//------------
		void SetInfo(const T & info);
		T & GetInfo();
	protected:
		virtual ~InfoLambdaOp();
		std::string ID;
		T info;
	private:
		InfoLambdaOp(const InfoLambdaOp&) = default;
		InfoLambdaOp& operator=(const InfoLambdaOp&) = default;
	};

	//------------

	template<typename T>
	InfoLambdaOp<T>::InfoLambdaOp(const std::string & ID, const T & info, const std::function<void()> & op, bool isHold)
		:info(info), ID(ID), LambdaOp(op, isHold) {
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
}
#endif // !_INFO_LAMBDA_OP_H_
