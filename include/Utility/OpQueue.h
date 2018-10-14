#ifndef _OP_QUEUE_H_
#define _OP_QUEUE_H_

#include <Utility/Operation.h>
#include <list>

namespace Ubpa {
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
}

#endif // !_OP_QUEUE_H_
