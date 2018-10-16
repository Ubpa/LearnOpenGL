#include <Utility/Config.h>
#include <regex>

using namespace Ubpa;
using namespace std;

Config::Config()
	:valid(false) { }

Config::Config(const std::string & fileName) {
	Load(fileName);
}

bool Config::Load(const std::string & fileName) {
	File file(fileName, File::READ);
	valid = file.IsValid();
	if (!valid) {
		printf("Config file [%s] can't open\n", fileName.c_str());
		return false;
	}
		
	string data = file.ReadAll();
	valid = Decode(data);
	if (!valid) {
		printf("Confige file [%s] fail.\n", data.c_str());
		return false;
	}

	return true;
}

bool Config::IsValid() {
	return valid;
}

bool Config::Decode(const string & data) {
	string blank = "(?:\\s*)";
	string lval = "(\\w+)";
	string rval = "(\\S+)";
	string exp = "(?:" + lval + blank + "=" + blank + rval + blank + ";)";
	string note = "(?:\\#[^\\n]*)";
	string line = "(?:" + blank + exp + "?" + blank + note + "?)";
	string neLine = "(?:" + line + "\\n)";
	string eLine = "(?:" + line + "|\\n)";
	string code = neLine + "*" + eLine;
	regex pattern(code);
	
	smatch result;
	
	if (!regex_match(data, result, pattern)) {
		printf("ERROR : The format is not correct.\n");
		return false;
	}

	for (size_t i = 1; i + 1 < result.size(); i += 2)
		directory.Register(result[i].str(), result[i + 1].str());
	return true;
}

string * Config::GetPtr(const std::string & id) {
	return directory.GetPtr(id);
}