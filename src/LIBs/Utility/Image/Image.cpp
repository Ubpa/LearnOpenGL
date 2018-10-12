#include <Utility/Image.h>


Image::Image(const char * fileName, int req_comp) {
	stbi_load(fileName, &width, &height, &channel, req_comp);
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

void Image::Free() {
	if (data != NULL) {
		stbi_image_free(data);
		data = NULL;
	}
}