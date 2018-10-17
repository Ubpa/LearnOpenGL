#include <Utility/File.h>

#include <cstdlib>
#include <cstdarg>

using namespace std;
using namespace Ubpa;


File::File(const std::string & fileName, Mode mode) {
	this->fileName = fileName;
	this->mode = mode;
	pF = fopen(fileName.c_str(), ModeToStr(mode).c_str());
	if (pF == NULL)
		fprintf(stderr, "%s open error\n", fileName.c_str());
}

File::~File() {
	if (pF != NULL) {
		fclose(pF);
		pF = NULL;
	}
}

bool File::Check(Mode mode) {
	if (mode == this->mode && IsValid())
		return true;
	else
		return false;
}

int File::Printf(const std::string & format, ...) {
	if (!Check(WRITE)) {
		fprintf(stderr, "%s can't not write\n", fileName.c_str());
		return -1;
	}
	va_list args;
	const char * cstr_format = format.c_str();
	va_start(args, cstr_format);
	int rst = vfprintf(pF, format.c_str(), args);
	va_end(args);
	return rst;
}

int File::Scanf(const std::string & format, ...) {
	if (!Check(READ)) {
		fprintf(stderr, "%s can't not read\n", fileName.c_str());
		return -1;
	}
	va_list args;
	const char * cstr_format = format.c_str();
	va_start(args, cstr_format);
	int rst = vfscanf(pF, format.c_str(), args);
	va_end(args);
	return rst;
}

string File::ReadLine() {
	if (!Check(READ)) {
		fprintf(stderr, "%s can't not read\n", fileName.c_str());
		return "";
	}
	char buffer[1024] = "";
	fgets(buffer, 1024, pF);
	return buffer;
}

string File::ReadAll() {
	if (!Check(READ)) {
		fprintf(stderr, "%s can't not read\n", fileName.c_str());
		return "";
	}
	string rst;
	while (!IsEnd())
		rst += ReadLine();
	return rst;
}

bool File::IsEnd() {
	return feof(pF);
}

bool File::IsValid() {
	return pF != NULL;
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
