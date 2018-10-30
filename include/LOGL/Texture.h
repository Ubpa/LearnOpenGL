#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <Utility/Image.h>
#include <vector>
#include <string>

namespace LOGL {
	class Texture {
	public:
		Texture(size_t ID = 0);
		Texture(const std::vector<std::string> & skybox);
		Texture(const std::string & path, bool flip = false, bool gammaCorrection = false);

		bool Load(const std::vector<std::string> & skybox);
		bool Load(const std::string & path, bool flip = false, bool gammaCorrection = false);

		bool Use(size_t id = 0);

		size_t GetID() const;
		bool IsValid() const;
	private:

		size_t ID;
		bool isValid;
	};
}

#endif // !_TEXTURE_H_
