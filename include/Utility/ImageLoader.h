#ifndef _IMAGE_LOADER_H_
#define _IMAGE_LOADER_H_

#define STB_IMAGE_IMPLEMENTATION
#include <Utility/stb_image.h>

namespace Ubpa {
	class ImageLoader {
	public:
		static ImageLoader * GetInstance();
		//------------
		unsigned char * Load(const char * fileName, int & width, int & height, int & numChannels, int req_comp = 0);
	private:
		ImageLoader();
		ImageLoader(const ImageLoader&);
		ImageLoader& operator=(const ImageLoader&);
		//------------
		static ImageLoader * instance;
	};
}

#endif // !_IMAGE_LOADER_H_
