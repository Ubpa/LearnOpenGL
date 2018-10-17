#ifndef _DEFINE_H_
#define _DEFINE_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <LOGL/CommonDefine.h>

namespace Define {
	const std::string str_Chapter = "01_Introduction";
	const std::string str_Subchapter = "07_Camera";

	const std::string str_ChangeTexture = "changeTexture";

	const int textureWarpMode[] = {
		GL_REPEAT, GL_MIRRORED_REPEAT, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_EDGE
	};
}

#endif // !_DEFINE_H_


