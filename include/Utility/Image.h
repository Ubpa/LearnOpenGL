#ifndef _IMAGE_H_
#define _IMAGE_H_

#define STB_IMAGE_IMPLEMENTATION
#include <Utility/stb_image.h>

class Image {
public:
	Image(const char * fileName, int req_comp = 0);
	~Image();
	//------------
	bool IsValid() const;
	unsigned char * GetData();
	int GetWidth() const;
	int GetHeight() const;
	int GetChannel() const;
	//------------
	void Free();
private:
	unsigned char * data;
	int width;
	int height;
	int channel;
};

#endif // !_IMAGE_H_
