#ifndef _LSTORAGE_H_
#define _LSTORAGE_H_

#include <map>
#include <string>

namespace Ubpa {
	// Local Storage
	template<typename T>
	class LStorage {
	public:
		// ��һ��ע�᷵�� true, �������� false
		bool Register(const std::string & uniqueID, const T & item);

		bool Unregister(const std::string & uniqueID);

		T * GetPtr(const std::string & uniqueID);

	private:
		std::map<std::string, T> directory;
	};

	//-----------
	template<typename T>
	bool LStorage<T>::Register(const std::string & uniqueID, const T & item) {
		auto target = directory.find(uniqueID);
		if (target != directory.end()) {
			target->second = item;
			return false;
		}

		directory[uniqueID] = item;
		return true;
	}

	template<typename T>
	bool LStorage<T>::Unregister(const std::string & uniqueID) {
		auto target = directory.find(uniqueID);
		if (target == directory.end())
			return false;

		directory.erase(target);
		return true;
	}

	template<typename T>
	T * LStorage<T>::GetPtr(const std::string & uniqueID) {
		auto target = directory.find(uniqueID);
		if (target == directory.end())
			return NULL;

		return &(target->second);
	}
}

#endif // !_LSTORAGE_H_
