#ifndef _STORAGE_H_
#define _STORAGE_H_

#include <Utility/Singleton.h>
#include <Utility/LStorage.h>

namespace Ubpa{
	template<typename T>
	class GStorage final : public Singleton<GStorage<T>>, public LStorage<T>{
		friend class Singleton<GStorage<T>>;
	public:
		static GStorage<T> * GetInstance() {
			return Singleton<GStorage<T>>::GetInstance();
		}
	private:
		GStorage() = default;
		virtual ~GStorage() = default;
	};
}

#endif