#include <Utility/ImageLoader.h>

using namespace Ubpa;

ImageLoader* ImageLoader::instance = new ImageLoader();
ImageLoader* ImageLoader::GetInstance() { return instance; }
//------------
unsigned char * ImageLoader::Load(const char * fileName, int & width, int & height, int & numChannels, int req_comp) {
	return stbi_load(fileName, &width, &height, &numChannels, req_comp);
}