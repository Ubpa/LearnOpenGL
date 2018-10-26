#include <LOGL/Texture.h>
#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace LOGL;
using namespace Ubpa;
using namespace std;

Texture::Texture(const char * path, bool flip){
	Image img(path, flip);
	isValid = img.IsValid();
	if (isValid == false) {
		printf("Texture [%s] load failed\n", path);
		return;
	}
	InitTexture(img);
}

Texture::Texture(const vector<string> & skybox) {
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id);

	// loads a cubemap texture from 6 individual texture faces
	// order:
	// +X (right)
	// -X (left)
	// +Y (top)
	// -Y (bottom)
	// +Z (front) 
	// -Z (back)
	// -------------------------------------------------------
	isValid = true;
	for (size_t i = 0; i < skybox.size(); i++)
	{
		Image img(skybox[i].c_str());
		if (!img.IsValid()){
			printf("Cubemap texture failed to load at path: %s\n", skybox[i].c_str());
			isValid = false;
			return;
		}
		
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, img.GetWidth(), img.GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, img.GetConstData());
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

size_t Texture::GetID() const{
	return id;
}

bool Texture::IsValid() const{
	return isValid;
}

void Texture::InitTexture(const Image & img) {
	GLenum format;
	int nrComponents = img.GetChannel();
	if (nrComponents == 1)
		format = GL_RED;
	else if (nrComponents == 3)
		format = GL_RGB;
	else if (nrComponents == 4)
		format = GL_RGBA;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	/*
	@1 纹理目标
	@2 多级渐远纹理的级别 （0 为基本级别)
	@3 纹理格式
	@4 width
	@5 height
	@6 0 (历史遗留问题)
	@7 源图格式
	@8 源图类型
	@9 图像数据
	*/
	glTexImage2D(GL_TEXTURE_2D, 0, format, img.GetWidth(), img.GetHeight(), 0, format, GL_UNSIGNED_BYTE, img.GetConstData());

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}