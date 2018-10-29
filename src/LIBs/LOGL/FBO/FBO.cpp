#include <LOGL/FBO.h>

#include <glad/glad.h>
#include <cstdio>

using namespace LOGL;
using namespace std;

FBO::FBO(size_t width, size_t height, ENUM_TYPE type)
	: type(type) {
	switch (type)
	{
	case LOGL::FBO::ENUM_TYPE_BASIC:
		if (!GenFBO_BASIC(width, height))
			printf("GenFBO_BASIC fail\n");
		break;
	default:
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

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("Framebuffer is not complete!\n");
		isValid = false;
	}
	else
		isValid = true;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return isValid;
}

size_t FBO::GetID() const {
	if (!isValid)
		return 0;
	
	return ID;
}

size_t FBO::IsValid() const {
	return isValid;
}

size_t FBO::GetColorBufferID() const {
	if (type != ENUM_TYPE_BASIC)
		return 0;
	
	return colorBufferID;
}