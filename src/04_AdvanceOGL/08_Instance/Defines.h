#ifndef _DEFINE_H_
#define _DEFINE_H_

#include <LOGL/CommonDefine.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Define {
	const std::string str_Chapter = "04_AdvanceOGL";
	const std::string str_Subchapter = "08_Instance";

	//------------ shader
	const std::string str_ShaderPrefix = "/data/shaders/" + str_Chapter + "/" + str_Subchapter + "/";

	const std::string str_Light_vs = str_ShaderPrefix + "light.vs";
	const std::string str_Light_fs = str_ShaderPrefix + "light.fs";

	const std::string str_Lighting_vs = str_ShaderPrefix + "lighting.vs";
	const std::string str_Lighting_fs = str_ShaderPrefix + "lighting.fs";

	const std::string str_Nanosuit_vs = str_ShaderPrefix + "nanosuit.vs";
	const std::string str_Nanosuit_fs = str_ShaderPrefix + "nanosuit.fs";

	const std::string str_SingleColor_vs = str_ShaderPrefix + "singleColor.vs";
	const std::string str_SingleColor_fs = str_ShaderPrefix + "singleColor.fs";

	const std::string str_Transparent_vs = str_ShaderPrefix + "transparent.vs";
	const std::string str_Transparent_fs = str_ShaderPrefix + "transparent.fs";

	const std::string str_PostProcess_vs = str_ShaderPrefix + "postProcess.vs";
	const std::string str_PostProcess_fs = str_ShaderPrefix + "postProcess.fs";

	const std::string str_Skybox_vs = str_ShaderPrefix + "skybox.vs";
	const std::string str_Skybox_fs = str_ShaderPrefix + "skybox.fs";

	const std::string str_Reflect_vs = str_ShaderPrefix + "reflect.vs";
	const std::string str_Reflect_fs = str_ShaderPrefix + "reflect.fs";

	const std::string str_Refract_vs = str_ShaderPrefix + "refract.vs";
	const std::string str_Refract_fs = str_ShaderPrefix + "refract.fs";

	const std::string str_House_vs = str_ShaderPrefix + "house.vs";
	const std::string str_House_fs = str_ShaderPrefix + "house.fs";
	const std::string str_House_gs = str_ShaderPrefix + "house.gs";

	const std::string str_Nanosuit_Explode_vs = str_ShaderPrefix + "nanosuit_explode.vs";
	const std::string str_Nanosuit_Explode_fs = str_ShaderPrefix + "nanosuit_explode.fs";
	const std::string str_Nanosuit_Explode_gs = str_ShaderPrefix + "nanosuit_explode.gs";

	const std::string str_Nanosuit_Normal_vs = str_ShaderPrefix + "nanosuit_normal.vs";
	const std::string str_Nanosuit_Normal_fs = str_ShaderPrefix + "nanosuit_normal.fs";
	const std::string str_Nanosuit_Normal_gs = str_ShaderPrefix + "nanosuit_normal.gs";

	const std::string str_Instance_vs = str_ShaderPrefix + "instance.vs";
	const std::string str_Instance_fs = str_ShaderPrefix + "instance.fs";


	const std::string str_LightColor = "lightColor";
	const std::string str_LightPos = "lightPos";
	const std::string str_PostProcess = "postProcessShader";


	//------------ data
	// positions of the point lights
	const glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	const glm::vec3 vegetationp[] = {
		glm::vec3(-1.5f, 0.0f, -0.48f),
		glm::vec3(1.5f, 0.0f, 0.51f),
		glm::vec3(0.0f, 0.0f, 0.7f),
		glm::vec3(-0.3f, 0.0f, -2.3f),
		glm::vec3(0.5f, 0.0f, -0.6f)
	};

	const float data_ScreanVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	const float data_House[] = {
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
		-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
	};
}

#endif // !_DEFINE_H_


