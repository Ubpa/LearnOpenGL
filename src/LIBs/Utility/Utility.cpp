#include <Ubpa/Utility.h>

using namespace std;

namespace Ubpa{
	ArgManager::ArgManager(int argc, char** argv) {
		string tag;
		for (int i = 1; i < argc; i++) {
			if (argv[i][0] == '-') {
				tag = string(&(argv[i][1]));
				pairs[tag] = vector<string>();
			}
			else if (tag.size() != 0)
				pairs[tag].push_back(argv[i]);
			else
				perror("arg format error\n");
		}
	}
	
	vector<string> ArgManager::GetAll(string key) {
		return pairs[key];
	}

	string ArgManager::Get(string key, unsigned int idx, string defaultValue) {
		vector<string> value = pairs[key];
		if(idx >= value.size())
			return defaultValue;
		else
			return value[idx];
	}
	//------------
	File::File(const char * fileName, Mode mode) {
		this->fileName = fileName;
		this->mode = mode;
		pF = fopen(fileName, ModeToStr(mode).c_str());
		if (pF == NULL)
			fprintf(stderr, "%s open error\n", fileName);
	}
	File::~File() {
		if (pF != NULL)
			fclose(pF);
	}
	
	bool File::Check(Mode mode) {
		if (mode == this->mode && pF != NULL)
			return true;
		else
			return false;
	}
	
	int File::Printf(char* format, ...) {
		if (!Check(WRITE)) {
			fprintf(stderr, "%s can't not write\n", fileName.c_str());
			return -1;
		}
		va_list args;
		va_start(args, format);
		int rst = vfprintf(pF, format, args);
		va_end(args);
		return rst;
	}
	
	int File::Scanf(char * format, ...) {
		if (!Check(READ)) {
			fprintf(stderr, "%s can't not read\n", fileName.c_str());
			return -1;
		}
		va_list args;
		va_start(args, format);
		int rst = vfscanf(pF, format, args);
		va_end(args);
		return rst;
	}
	
	string File::ReadLine() {
		if (Check(READ)) {
			fprintf(stderr, "file can't not read\n");
			return "";
		}
		char buffer[1024];
		Scanf("%s\n", buffer);
		return buffer;
	}
	
	string File::ModeToStr(Mode mode) {
		switch (mode)
		{
		case READ:
			return "r";
			break;
		case WRITE:
			return "w";
			break;
		default:
			return "";
			break;
		}
	}
};