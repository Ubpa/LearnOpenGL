#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <Utility/Image.h>

namespace LOGL {
	class Texture {
	public:
		Texture(const char * path, bool flip = false);
		size_t GetID();
		bool IsValid();
	private:
		void InitTexture();

		size_t id;
		bool isValid;
		Ubpa::Image img;
	};
}

#endif // !_TEXTURE_H_
