#ifndef _EVENT_MANAGER_H_
#define _EVENT_MANAGER_H_

#include <Utility/Operation.h>
#include <map>

namespace Ubpa {
	class EventManager {
	public:
		static EventManager * GetInstance();
		//------------
		void RegisterOp(int event, Ptr<Operation> & op);
		void RegisterOp(int event, Operation * op);
		void RegisterOp(int event, const std::function<void ()> & op);
		void Response(int event);
	private:
		EventManager();
		EventManager(const EventManager &);
		EventManager & operator=(const EventManager &);
		static EventManager * instance;
		//------------
		std::map<int, Ptr<OpQueue> > directory;
	};

}
#endif // !_EVENT_MANAGER_H_
