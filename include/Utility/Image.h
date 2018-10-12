#ifndef _IMAGE_H_
#define _IMAGE_H_

class Image {
public:
	Image();
	Image(const char * fileName, bool flip = false, int req_comp = 0);
	~Image();
	//------------
	bool IsValid() const;
	unsigned char * GetData();
	int GetWidth() const;
	int GetHeight() const;
	int GetChannel() const;
	//------------
	bool Load(const char * fileName, bool flip = false, int req_comp = 0);
	void Free();
private:
	unsigned char * data;
	int width;
	int height;
	int channel;
};

#endif // !_IMAGE_H_
