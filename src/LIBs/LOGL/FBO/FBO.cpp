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
	case LOGL::FBO::ENUM_TYPE_BASIC_FLOAT:
		if (!GenFBO_BASIC_FLOAT(width, height))
			printf("GenFBO_BASIC_FLOAT fail\n");
		break;
	case LOGL::FBO::ENUM_TYPE_MSAA:
		if (!GenFBO_MSAA(width, height))
			printf("GenFBO_MSAA fail\n");
		break;
	case LOGL::FBO::ENUM_TYPE_COLOR:
		if (!GenFBO_COLOR(width, height))
			printf("GenFBO_COLOR fail\n");
		break;
	case LOGL::FBO::ENUM_TYPE_DEPTH:
		if (!GenFBO_DEPTH(width, height))
			printf("GenFBO_DEPTH fail\n");
		break;
	case LOGL::FBO::ENUM_TYPE_CUBE_DEPTH:
		if (!GenFBO_CUBE_DEPTH(width, height))
			printf("GenFBO_CUBE_DEPTH fail\n");
		break;
	default:
		printf("ERROR: FBO type not know\n");
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

bool FBO::GenFBO_BASIC_FLOAT(size_t width, size_t height) {
	glGenFramebuffers(1, &ID);
	glBindFramebuffer(GL_FRAMEBUFFER, ID);

	glGenTextures(1, &colorBufferID);
	glBindTexture(GL_TEXTURE_2D, colorBufferID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
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
	const size_t samples = 4;
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

bool FBO::GenFBO_DEPTH(size_t width, size_t height) {
	glGenFramebuffers(1, &ID);
	// create depth texture
	glGenTextures(1, &depthBufferID);
	glBindTexture(GL_TEXTURE_2D, depthBufferID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBufferID, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	isValid = IsComplete();
	if (!isValid)
		printf("Framebuffer is not complete!\n");

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return isValid;
}

bool FBO::GenFBO_CUBE_DEPTH(size_t width, size_t height) {
	glGenFramebuffers(1, &ID);

	glGenTextures(1, &depthBufferID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthBufferID);
	for (GLuint i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
			width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthBufferID, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

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

bool FBO::PassTo(const FBO & fbo, ENUM_PASS_TYPE passType) const {
	if (!isValid || !fbo.isValid)
		return false;
	glBindFramebuffer(GL_READ_FRAMEBUFFER, ID);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo.ID);
	glBlitFramebuffer(0, 0, width, height, 0, 0, fbo.width, fbo.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	return true;
}

size_t FBO::GetColorBufferID() const {
	if (!isValid)
		return 0;

	if (type != ENUM_TYPE_BASIC
		&& type != ENUM_TYPE_MSAA
		&& type != ENUM_TYPE_COLOR
		&& type != ENUM_TYPE_BASIC_FLOAT)
		return 0;
	
	return colorBufferID;
}

size_t FBO::GetDepthBufferID() const {
	if (!isValid)
		return 0;

	if (type != ENUM_TYPE_DEPTH
		&& type != ENUM_TYPE_CUBE_DEPTH)
		return 0;

	return depthBufferID;
}

bool FBO::Use() {
	if (!isValid)
		return false;

	glBindFramebuffer(GL_FRAMEBUFFER, ID);
	return true;
}

void FBO::UseDefault() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}