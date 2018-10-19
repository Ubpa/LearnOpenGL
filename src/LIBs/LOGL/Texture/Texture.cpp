#include <LOGL/Texture.h>
#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace LOGL;

Texture::Texture(const char * path, bool flip)
:img(path, flip){
	isValid = img.IsValid();
	if (isValid == false) {
		printf("Texture [%s] load failed\n", path);
		return;
	}
	InitTexture();
}
size_t Texture::GetID() {
	return id;
}
bool Texture::IsValid() {
	return isValid;
}

void Texture::InitTexture() {
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
	@1 ����Ŀ��
	@2 �༶��Զ����ļ��� ��0 Ϊ��������)
	@3 �����ʽ
	@4 width
	@5 height
	@6 0 (��ʷ��������)
	@7 Դͼ��ʽ
	@8 Դͼ����
	@9 ͼ������
	*/
	glTexImage2D(GL_TEXTURE_2D, 0, format, img.GetWidth(), img.GetHeight(), 0, format, GL_UNSIGNED_BYTE, img.GetData());

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	img.Free();
}