#include <Utility/Config.h>
#include <regex>
#include <sstream>
#include <vector>

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
	string strT = "(string)";
	string intT = "(int)";
	string floatT = "(float)";
	string blank = "(?:[ \t]*)";
	string vName = "([_a-zA-Z][_0-9a-zA-Z]*)";
	string strVal = "(?:\"([^\"]*)\")";
	string strExp = "(?:" + strT + blank + vName + blank + "=" + blank + strVal + blank + ";)";
	string intVal = "((?:[1-9][0-9]*)|(?:0))";
	string intExp = "(?:" + intT + blank + vName + blank + "=" + blank + intVal + blank + ";)";
	string floatVal = "((?:(?:0)|(?:[1-9][0-9]*))(?:(?:\\.[0-9]*)?))";
	string floatExp = "(?:" + floatT + blank + vName + blank + "=" + blank + floatVal + blank + ";)";
	string allExp = "(?:" + strExp + "|" + intExp + "|" + floatExp + ")";
	string note = "(?:\\#[^\\n]*)";
	string line = "(?:" + blank + "(?:" + allExp + "?)" + blank + "(?:" + note + "?)" + "\\n)";

	regex pattern(line);

	smatch result;
	
	if (!regex_match(data, result, pattern)) {
		printf("ERROR : The format is not correct.\n");
		return false;
	}
	
	vector<int> idxVec;
	for (size_t i = 1; i < result.size(); i++) {
		if (result[i].str().size() > 0) {
			printf("%s\n", result[i].str().c_str());
			idxVec.push_back(i);
		}
	}
	printf("num:%d\n", idxVec.size());
	for (size_t i = 0; i < idxVec.size(); i += 3) {
		size_t idx = idxVec[i];
		string type = result[idxVec[i]].str();
		string varName = result[idxVec[i + 1]].str();
		string valueStr = result[idxVec[i + 2]].str();
		if (type == "string") {
			printf("Config: %s = %s\n", varName.c_str(), valueStr.c_str());
			strDirectory.Register(varName, valueStr);
		}
		else if (type == "float") {
			istringstream iss(valueStr);
			float val;
			iss >> val;
			printf("Config: %s = %f\n", varName.c_str(), val);
			floatDirectory.Register(varName, val);
		}
		else if (type == "int") {
			istringstream iss(valueStr);
			int val;
			iss >> val;
			printf("Config: %s = %d\n", varName.c_str(), val);
			intDirectory.Register(varName, val);
		}
		else {
			printf("Error Type [%s] is unknown\nvarName : %s\nvalue : %s\n",
				result[idxVec[i]].str().c_str(), result[i+1].str().c_str(), result[i+2].str().c_str());
		}
	}
	return true;
}


string * Config::GetStrPtr(const std::string & id) {
	return strDirectory.GetPtr(id);
}
float * Config::GetFloatPtr(const std::string & id) {
	return floatDirectory.GetPtr(id);
}
int * Config::GetIntPtr(const std::string & id) {
	return intDirectory.GetPtr(id);
}