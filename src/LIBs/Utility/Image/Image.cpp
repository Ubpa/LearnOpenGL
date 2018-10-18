#include <Utility/Image.h>

#define STB_IMAGE_IMPLEMENTATION
#include <Utility/stb_image.h>

using namespace Ubpa;

Image::Image()
	:data(NULL), width(0), height(0), channel(0){ }

Image::Image(const char * fileName, bool flip, int req_comp) {
	data = NULL;
	Load(fileName, flip, req_comp);
}

Image::~Image() {
	Free();
}

//-----------

bool Image::IsValid() const{
	return data != NULL;
}

unsigned char * Image::GetData() {
	return data;
}

int Image::GetWidth() const{
	return width;
}

int Image::GetHeight() const{
	return height;
}

int Image::GetChannel() const{
	return channel;
}

//------------

bool Image::Load(const char * fileName, bool flip, int req_comp) {
	Free();
	stbi_set_flip_vertically_on_load(flip);
	data = stbi_load(fileName, &width, &height, &channel, req_comp);
	return IsValid();
}


void Image::Free() {
	if (data != NULL) {
		stbi_image_free(data);
		data = NULL;
		width = 0;
		height = 0;
		channel = 0;
	}
}