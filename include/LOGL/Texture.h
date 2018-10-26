#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <Utility/Image.h>
#include <vector>
#include <string>

namespace LOGL {
	class Texture {
	public:
		Texture(const std::vector<std::string> & skybox);
		Texture(const char * path, bool flip = false);
		size_t GetID() const;
		bool IsValid() const;
	private:
		void InitTexture(const Ubpa::Image & img);

		size_t id;
		bool isValid;
	};
}

#endif // !_TEXTURE_H_
