#ifndef _FILE_H_
#define _FILE_H_

#include <string>
#include <cstdio>

namespace Ubpa{
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
#endif//! _FILE_H_