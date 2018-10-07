#ifndef UTILITY_H_
#define UTILITY_H_

#include <map>
#include <string>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <cstdarg>

namespace Ubpa{
	class ArgManager{
	public:
		ArgManager(int argc, char** argv);
		std::vector<std::string> GetAll(std::string key);
		std::string Get(std::string key, unsigned int idx, std::string defaultValue = "");
	private:
		std::map<std::string, std::vector<std::string>> pairs;
	};
	
	class File{
	public:
		enum Mode {
			READ,
			WRITE
		};

		File(const char * fileName, Mode mode);

		~File();

		bool Check(Mode mode);

		int Printf(char* format, ...);

		int Scanf(char * format, ...);

		std::string ReadLine();
		std::string ReadAll();

	private:
		std::string ModeToStr(Mode mode);

		FILE * pF;
		Mode mode;
		std::string fileName;
	};
};
#endif