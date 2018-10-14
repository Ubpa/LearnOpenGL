#ifndef _EVENT_MANAGER_H_
#define _EVENT_MANAGER_H_

#include <Utility/OpQueue.h>
#include <functional>
#include <map>

namespace Ubpa {
	class EventManager {
	public:
		enum ENUM_EVENT {
			//KEY = 0x00000000 ~ 0x0000FFFF
			KEYBOARD_PRESS = 0x00010000,
			KEYBOARD_REPEAT = 0x00020000,
			KEYBOARD_RELEASE = 0x00040000,
			KEYBOARD = KEYBOARD_PRESS | KEYBOARD_REPEAT | KEYBOARD_RELEASE,
			MOUSE_SCROLL = 0x00080000,
			MOUSE_MOUVE = 0x00080001,
			WINDOW_ZOOM = 0x00080002,
		};
		//------------
		static EventManager * GetInstance();
		//------------
		void RegisterOp(size_t event, Ptr<Operation> & op);
		void RegisterOp(size_t event, Operation * op);
		void RegisterOp(size_t event, const std::function<void()> & op);
		void Response(size_t event);
	private:
		EventManager();
		EventManager(const EventManager &);
		EventManager & operator=(const EventManager &);
		static EventManager * instance;
		//------------
		std::map<size_t, Ptr<OpQueue> > directory;
	};
}
#endif // !_EVENT_MANAGER_H_
