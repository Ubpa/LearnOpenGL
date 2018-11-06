#include <LOGL/Texture.h>
#include <cstdio>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace LOGL;
using namespace Ubpa;
using namespace std;

const Texture Texture::InValid(0, ENUM_TYPE_NOT_VALID);

Texture::Texture(size_t width, size_t height) {
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
}

Texture::Texture(size_t ID, ENUM_TYPE type)
	: ID(ID), type(type) { }

Texture::Texture(ENUM_TYPE type)
	: Texture(0, type) { }

Texture::Texture(size_t width, size_t height, float const * data, size_t dataType, size_t srcFormat, size_t internalFormat) {
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, srcFormat, dataType, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	type = ENUM_TYPE_2D;
}

Texture::Texture(const std::string & path, bool flip, bool gammaCorrection){
	type = ENUM_TYPE_NOT_VALID;
	Load(path, flip, gammaCorrection);
}

Texture::Texture(const vector<string> & skybox) {
	type = ENUM_TYPE_NOT_VALID;
	Load(skybox);
}

size_t Texture::GetID() const{
	return ID;
}

bool Texture::IsValid() const{
	return ID != 0 && type != ENUM_TYPE_NOT_VALID;
}

bool Texture::Load(const std::vector<std::string> & skybox) {
	if (IsValid()) {
		printf("ERROR: The texture is valid already.\n");
		
		return false;
	}

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, ID);

	// loads a cubemap texture from 6 individual texture faces
	// order:
	// +X (right)
	// -X (left)
	// +Y (top)
	// -Y (bottom)
	// +Z (front) 
	// -Z (back)
	// -------------------------------------------------------
	for (size_t i = 0; i < skybox.size(); i++)
	{
		Image img(skybox[i].c_str());
		if (!img.IsValid()) {
			printf("Cubemap texture failed to load at path: %s\n", skybox[i].c_str());
			type = ENUM_TYPE_NOT_VALID;
			return false;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, img.GetWidth(), img.GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, img.GetConstData());
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	type = ENUM_TYPE_2D;
	UnBind();
	return true;
}

bool Texture::Load(const std::string & path, bool flip, bool gammaCorrection) {
	if (IsValid()) {
		printf("ERROR: The texture is valid already.\n");
		return false;
	}
	
	Image img(path.c_str(), flip);
	if (!img.IsValid()) {
		printf("ERROR: Texture [%s] load failed\n", path.c_str());
		type = ENUM_TYPE_NOT_VALID;
		return false;
	}

	GLenum internalFormat;
	GLenum dataFormat;
	int nrComponents = img.GetChannel();
	if (nrComponents == 1) {
		internalFormat = GL_RED;
		dataFormat = GL_RED;
	}
	else if (nrComponents == 3) {
		internalFormat = gammaCorrection ? GL_SRGB : GL_RGB;
		dataFormat = GL_RGB;
	}
	else if (nrComponents == 4) {
		internalFormat = gammaCorrection ? GL_SRGB_ALPHA : GL_RGBA;
		dataFormat = GL_RGBA;
	}

	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

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
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, img.GetWidth(), img.GetHeight(), 0, dataFormat, GL_UNSIGNED_BYTE, img.GetConstData());

	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	type = ENUM_TYPE_2D;
	UnBind();
	return true;
}

bool Texture::SetImg(const Image & img) {
	if (type != ENUM_TYPE_2D_DYNAMIC) {
		printf("ERROR: type[%s] can't set image\n", Type2Str(type).c_str());
		return false;
	}

	if (ID == 0)
		glGenTextures(1, &ID);

	GLenum format;
	int nrComponents = img.GetChannel();
	if (nrComponents == 1)
		format = GL_RED;
	else if (nrComponents == 3)
		format = GL_RGB;
	else if (nrComponents == 4)
		format = GL_RGBA;

	glBindTexture(GL_TEXTURE_2D, ID);
	glTexImage2D(GL_TEXTURE_2D, 0, format, img.GetWidth(), img.GetHeight(), 0, format, GL_UNSIGNED_BYTE, img.GetConstData());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	UnBind();
}

bool Texture::Use(size_t id) const{
	if (!IsValid())
		return false;

	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(Type2GL(type), ID);
	return true;
}

size_t Texture::Type2GL(ENUM_TYPE type) {
	switch (type)
	{
	case ENUM_TYPE_NOT_VALID:
		return 0;
	case ENUM_TYPE_2D:
	case ENUM_TYPE_2D_DYNAMIC:
		return GL_TEXTURE_2D;
	case ENUM_TYPE_CUBE_MAP:
		return GL_TEXTURE_CUBE_MAP;
	default:
		return 0;
	}
}

string Texture::Type2Str(ENUM_TYPE type) {
	switch (type)
	{
	case ENUM_TYPE_NOT_VALID:
		return "ENUM_TYPE_NOT_VALID";
	case ENUM_TYPE_2D:
		return "ENUM_TYPE_2D";
	case ENUM_TYPE_CUBE_MAP:
		return "ENUM_TYPE_CUBE_MAP";
	case ENUM_TYPE_2D_DYNAMIC:
		return "ENUM_TYPE_2D_DYNAMIC";
	default:
		return "UNKNOWN_TYPE";
	}
}

void Texture::UnBind() const{
	if (!IsValid())
		return;
	
	glBindTexture(Type2GL(type), 0);
}