#include <Utility/File.h>

#include <cstdlib>
#include <cstdarg>

using namespace std;
using namespace Ubpa;


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
	if (!Check(READ)) {
		fprintf(stderr, "%s can't not read\n", fileName.c_str());
		return "";
	}
	char buffer[1024];
	fgets(buffer, 1024, pF);
	return buffer;
}

string File::ReadAll() {
	if (!Check(READ)) {
		fprintf(stderr, "%s can't not read\n", fileName.c_str());
		return "";
	}
	string rst;
	while (!feof(pF))
		rst += ReadLine();
	return rst;
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
