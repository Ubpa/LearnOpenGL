#include <LOGL/FBO.h>

#include <glad/glad.h>
#include <cstdio>

using namespace LOGL;
using namespace std;

FBO::FBO(size_t width, size_t height, ENUM_TYPE type)
	: type(type), width(width), height(height) {
	colorBufferID = 0;
	switch (type)
	{
	case LOGL::FBO::ENUM_TYPE_BASIC:
		if (!GenFBO_BASIC(width, height))
			printf("GenFBO_BASIC fail\n");
		break;
	case LOGL::FBO::ENUM_TYPE_MSAA:
		if (!GenFBO_MSAA(width, height))
			printf("GenFBO_MSAA fail\n");
		break;
	case LOGL::FBO::ENUM_TYPE_COLOR:
		if (!GenFBO_COLOR(width, height))
			printf("GenFBO_COLOR fail\n");
		break;
	default:
		isValid = false;
		break;
	}
}

bool FBO::GenFBO_BASIC(size_t width, size_t height) {
	glGenFramebuffers(1, &ID);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);

	glGenTextures(1, &colorBufferID);
	glBindTexture(GL_TEXTURE_2D, colorBufferID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBufferID, 0);

	size_t RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	isValid = IsComplete();
	if (!isValid)
		printf("Framebuffer is not complete!\n");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return isValid;
}

bool FBO::GenFBO_MSAA(size_t width, size_t height) {
	const float samples = 4;
	glGenFramebuffers(1, &ID);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);

	glGenTextures(1, &colorBufferID);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, colorBufferID);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width, height, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, colorBufferID, 0);

	size_t RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);

	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	isValid = IsComplete();
	if (!isValid)
		printf("Framebuffer is not complete!\n");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return isValid;
}

bool FBO::GenFBO_COLOR(size_t width, size_t height) {
	glGenFramebuffers(1, &ID);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	// create a color attachment texture
	glGenTextures(1, &colorBufferID);
	glBindTexture(GL_TEXTURE_2D, colorBufferID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBufferID, 0);	// we only need a color buffer

	isValid = IsComplete();
	if (!isValid)
		printf("Framebuffer is not complete!\n");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return isValid;
}

size_t FBO::GetID() const {
	if (!isValid)
		return 0;
	
	return ID;
}

bool FBO::IsValid() const {
	return isValid;
}

bool FBO::IsComplete() const {
	if (!isValid)
		return false;

	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	auto rst = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (rst != GL_FRAMEBUFFER_COMPLETE) {
		printf("Framebuffer is not complete!\n");
		return false;
	}

	return true;
}

size_t FBO::GetColorBufferID() const {
	if (type != ENUM_TYPE_BASIC
		&& type != ENUM_TYPE_MSAA
		&& type != ENUM_TYPE_COLOR)
		return 0;
	
	return colorBufferID;
}

bool FBO::PassTo(const FBO & fbo, ENUM_PASS_TYPE passType) const {
	if (!isValid || !fbo.isValid)
		return false;
	glBindFramebuffer(GL_READ_FRAMEBUFFER, ID);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo.ID);
	glBlitFramebuffer(0, 0, width, height, 0, 0, fbo.width, fbo.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	return true;
}

bool FBO::Use() {
	if (!isValid)
		return false;

	glBindTexture(GL_TEXTURE_2D, ID);
}

void FBO::UseDefault() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}