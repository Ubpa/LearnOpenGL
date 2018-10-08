#ifndef _ARGMANAGER_H_
#define _ARGMANAGER_H_

#include <string>
#include <vector>
#include <map>

namespace Ubpa{
	class ArgManager{
	public:
		ArgManager(int argc, char** argv);
		std::vector<std::string> GetAll(std::string key);
		std::string Get(std::string key, size_t idx, std::string defaultValue = "");
	private:
		std::map<std::string, std::vector<std::string>> pairs;
	};
};
#endif//! _ARGMANAGER_H_