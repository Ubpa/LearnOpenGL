#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <Utility/File.h>
#include <Utility/LStorage.h>

namespace Ubpa {
	class Config {
	public:
		Config();
		Config(const std::string & fileName);
		bool Load(const std::string & fileName);
		std::string * GetStrPtr(const std::string & id);
		float * GetFloatPtr(const std::string & id);
		int * GetIntPtr(const std::string & id);
		bool IsValid();
	private:
		bool Config::Decode(const std::string & data);
		//------------
		LStorage<std::string> strDirectory;
		LStorage<float> floatDirectory;
		LStorage<int> intDirectory;
		bool valid;
	};
}

#endif // !_CONFIG_H_
