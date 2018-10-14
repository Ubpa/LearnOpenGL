#ifndef _STORAGE_H_
#define _STORAGE_H_

#include <Utility/Singleton.h>
#include <string>
#include <cstdlib>
#include <map>
#include <memory>

namespace Ubpa{
	template<typename T>
	class Storage final : public Singleton<Storage<T>>{
		friend class Singleton<Storage<T>>;
	public:
		static Storage<T> * GetInstance() {
			return Singleton<Storage<T>>::GetInstance();
		}

		bool Register(const std::string & uniqueID, const T & item){
			if(directory.find(uniqueID) != directory.end())
				return false;
			
			directory[uniqueID] = item;
			return true;
		}
		
		bool Unregister(const std::string & uniqueID){
			auto target = directory.find(uniqueID);
			if(target == directory.end())
				return false;
			
			directory.erase(target);
			return true;
		}
		
		T * Get(const std::string & uniqueID){
			auto target = directory.find(uniqueID);
			if(target == directory.end())
				return NULL;
			
			return &(target->second);
		}
		
		std::string GenID(){
			std::string rst(32, '0');
			do{
				for(auto c : rst)
					c = '0' + rand() % 10;
			}while(Get(rst) != NULL);
			return rst;
		}
		
	private:
		std::map<std::string, T> directory;
		Storage() = default;
		virtual ~Storage() = default;
	};
}

#endif