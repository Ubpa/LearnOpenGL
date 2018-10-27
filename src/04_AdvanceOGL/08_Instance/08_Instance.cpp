#include <ROOT_PATH.h>

#include <GLFW/Glfw.h>

#include <Utility/Image.h>
#include <Utility/GStorage.h>
#include <Utility/LambdaOp.h>
#include <Utility/Config.h>
#include <Utility/Sphere.h>
#include <Utility/Cube.h>
#include <Utility/OpNode.h>

#include <LOGL/Camera.h>
#include <LOGL/Texture.h>
#include <LOGL/Model.h>
#include <LOGL/VAO.h>

#include "Defines.h"
#include "RegisterInput.h"

using namespace LOGL;
using namespace std;
using namespace Ubpa;
using namespace Define;

Config * DoConfig();

int main(int argc, char ** argv) {
	Config * config = DoConfig();
	string rootPath = *config->GetStrPtr(str_RootPath);
	GStorage<Config *>::GetInstance()->Register(str_MainConfig, config);
	GStorage<string>::GetInstance()->Register(str_RootPath, rootPath);
	
	
	//------------ 窗口
	float ratioWH = (float)val_windowWidth / (float)val_windowHeight;
	string windowTitle = str_Chapter + "/" + str_Subchapter;
	Glfw::GetInstance()->Init(val_windowWidth, val_windowHeight, windowTitle.c_str());
	Glfw::GetInstance()->LockCursor();
	

	//------------ 平面
	size_t panelVAO = VAO(&(planeVertices[0]), sizeof(planeVertices), { 3,3,2 }).GetID();


	//------------ 天空盒
	Cube skybox;
	size_t VAO_Skybox = VAO(skybox.GetVertexArr(), skybox.GetVertexArrSize(), { 3 },
		skybox.GetIndexArr(), skybox.GetIndexArrSize()).GetID();


	//------------ 灯
	size_t lightVAO = VAO(&(data_vertices[0]), sizeof(data_vertices), { 3, 2 }).GetID();


	//------------ trasparent panel
	size_t transparentVAO = VAO(&(transparentVertices[0]), sizeof(transparentVertices), { 3,3,2 }).GetID();


	//------------ 屏幕
	size_t screanVAO = VAO(&(data_ScreanVertices[0]), sizeof(data_ScreanVertices), { 2,2 }).GetID();


	//------------ 正方体
	Cube cube;
	vector<VAO::VBO_DataPatch> cube_Vec_VBO_Data_Patch = {
		{cube.GetVertexArr(), cube.GetVertexArrSize(), 3},
		{cube.GetNormalArr(), cube.GetNormalArrSize(), 3},
		{cube.GetTexCoordsArr(), cube.GetTexCoordsArrSize(), 2},
	};
	size_t cubeVAO = VAO(cube_Vec_VBO_Data_Patch, cube.GetIndexArr(), cube.GetIndexArrSize()).GetID();


	//------------ 球
	Sphere sphere = Sphere(30);
	vector<VAO::VBO_DataPatch> sphere_Vec_VBO_Data_Patch = {
		{sphere.GetVertexArr(), sphere.GetVertexArrSize(), 3},
		{sphere.GetNormalArr(), sphere.GetNormalArrSize(), 3},
		{sphere.GetTexCoordsArr(), sphere.GetTexCoordsArrSize(), 2},
	};
	size_t sphereVAO = VAO(sphere_Vec_VBO_Data_Patch, sphere.GetIndexArr(), sphere.GetIndexArrSize()).GetID();


	//------------ 实例化球
	const size_t instanceNum = 1000;
	const float radius = 200.0f;
	glm::vec3 offset[instanceNum];
	for (size_t i = 0; i < instanceNum; i++) {
		offset[i].y = radius * 0.2 * (rand() / (float)RAND_MAX - 0.5);
		offset[i].x = radius * (cosf(2 * PI / instanceNum * i) + 0.02 * (rand() / (float)RAND_MAX - 0.5));
		offset[i].z = radius * (sinf(2 * PI / instanceNum * i) + 0.02 * (rand() / (float)RAND_MAX - 0.5));
	}
	vector<VAO::VBO_DataPatch> instancedSphere_Vec_VBO_Data_Patch = {
		{sphere.GetVertexArr(), sphere.GetVertexArrSize(), 3},
		{sphere.GetTexCoordsArr(), sphere.GetTexCoordsArrSize(), 2},
		{&(offset[0].x), sizeof(offset), 3}
	};
	VAO VAO_InstancedSphere(instancedSphere_Vec_VBO_Data_Patch, sphere.GetIndexArr(), sphere.GetIndexArrSize(), {0,0,1});


	//------------ 房子
	VAO VAO_House(data_House, sizeof(data_House), { 2,3 });

	//------------ 纹理
	const int textureNum = 7;
	size_t textureID[textureNum];
	string imgName[textureNum] = {
		rootPath + str_Img_Container2,
		rootPath + str_Img_Earth,
		rootPath + str_Img_Container2_Specular,
		rootPath + str_Img_Matrix,
		rootPath + str_Img_Marble,
		rootPath + str_Img_Grass,
		rootPath + str_Img_Window
	}; 
	bool flip[textureNum] = { false, true, false, true, false, true, false };
	for (size_t i = 0; i < textureNum; i++) {
		Texture tex(imgName[i].c_str());
		if (!tex.IsValid()) {
			printf("Load textureID[%s] fail.\n", imgName[i].c_str());
			return 1;
		}

		textureID[i] = tex.GetID();
	}
	
	vector<string> skyboxImgPath(6);
	for (size_t i = 0; i < 6; i++)
		skyboxImgPath[i] = rootPath + str_Vec_Img_Skybox[i];
	Texture skyboxTexture(skyboxImgPath);
	if (!skyboxTexture.IsValid()) {
		printf("skyboxTexture is not valid!\n");
		return 1;
	}

	//------------ 相机
	float moveSpeed = *config->GetFloatPtr(config_CameraMoveSpeed);
	float rotateSpeed = *config->GetFloatPtr(config_CameraRotateSensitivity);
	Camera mainCamera(ratioWH, moveSpeed, rotateSpeed, glm::vec3(0.0f, 0.0f, 4.0f));
	GStorage<Camera *>::GetInstance()->Register(str_MainCamera.c_str(), &mainCamera);
	
	//------------ nanosuit 着色器
	string nanosuit_vs = rootPath + str_Nanosuit_vs;
	string nanosuit_fs = rootPath + str_Nanosuit_fs;
	Shader nanosuitShader(nanosuit_vs.c_str(), nanosuit_fs.c_str());
	if (!nanosuitShader.IsValid()) {
		cout << "nanosuitShader is not Valid\n";
		return 1;
	}

	//------------ transparent 着色器
	string transparent_vs = rootPath + str_Transparent_vs;
	string transparent_fs = rootPath + str_Transparent_fs;
	Shader transparentShader(transparent_vs.c_str(), transparent_fs.c_str());
	if (!transparentShader.IsValid()) {
		cout << "transparentShader is not Valid\n";
		return 1;
	}
	transparentShader.Use();
	transparentShader.SetInt("texture1", 0);

	//------------ 单色着色器
	string singleColor_vs = rootPath + str_SingleColor_vs;
	string singleColor_fs = rootPath + str_SingleColor_fs;
	Shader singleColorShader(singleColor_vs.c_str(), singleColor_fs.c_str());
	if (!singleColorShader.IsValid()) {
		cout << "singleColorShader is not Valid\n";
		return 1;
	}


	//------------ 光源着色器
	string light_vs = rootPath + str_Light_vs;
	string light_fs = rootPath + str_Light_fs;
	Shader lightShader(light_vs.c_str(), light_fs.c_str());
	if (!lightShader.IsValid()) {
		cout << "lightShader is not Valid\n";
		return 1;
	}
	lightShader.SetVec3f("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
	

	//------------ 天空盒着色器
	string skybox_vs = rootPath + str_Skybox_vs;
	string skybox_fs = rootPath + str_Skybox_fs;
	Shader skyboxShader(skybox_vs.c_str(), skybox_fs.c_str());
	if (!skyboxShader.IsValid()) {
		cout << "skyboxShader is not Valid\n";
		return 1;
	}
	skyboxShader.SetInt("skybox", 0);


	//------------ 后期处理着色器
	string postProcess_vs = rootPath + str_PostProcess_vs;
	string postProcess_fs = rootPath + str_PostProcess_fs;
	Shader postProcessShader(postProcess_vs.c_str(), postProcess_fs.c_str());
	if (!postProcessShader.IsValid()) {
		cout << "postProcessShader is not Valid\n";
		return 1;
	}
	postProcessShader.SetInt("screenTexture", 0);
	postProcessShader.SetInt("mode", 0);
	GStorage<Shader *>::GetInstance()->Register(str_PostProcess, &postProcessShader);


	//------------ 反射着色器
	string reflect_vs = rootPath + str_Reflect_vs;
	string reflect_fs = rootPath + str_Reflect_fs;
	Shader reflectShader(reflect_vs.c_str(), reflect_fs.c_str());
	if (!reflectShader.IsValid()) {
		cout << "reflectShader is not Valid\n";
		return 1;
	}
	reflectShader.SetInt("skybox", 0);


	//------------ 折射着色器
	string refract_vs = rootPath + str_Refract_vs;
	string refract_fs = rootPath + str_Refract_fs;
	Shader refractShader(refract_vs.c_str(), refract_fs.c_str());
	if (!refractShader.IsValid()) {
		cout << "refractShader is not Valid\n";
		return 1;
	}
	refractShader.SetInt("skybox", 0);

	
	//------------ 房子着色器
	string house_vs = rootPath + str_House_vs;
	string house_fs = rootPath + str_House_fs;
	string house_gs = rootPath + str_House_gs;
	Shader houseShader(house_vs.c_str(), house_fs.c_str(), house_gs.c_str());
	if (!houseShader.IsValid()) {
		cout << "houseShader is not Valid\n";
		return 1;
	}


	//------------ nanosuit爆炸着色器
	string nanosuit_explode_vs = rootPath + str_Nanosuit_Explode_vs;
	string nanosuit_explode_fs = rootPath + str_Nanosuit_Explode_fs;
	string nanosuit_explode_gs = rootPath + str_Nanosuit_Explode_gs;
	Shader nanosuitExplodeShader(nanosuit_explode_vs.c_str(), 
		nanosuit_explode_fs.c_str(), nanosuit_explode_gs.c_str());
	if (!nanosuitExplodeShader.IsValid()) {
		cout << "nanosuitExplodeShader is not Valid\n";
		return 1;
	}


	//------------ nanosuit向量着色器
	string nanosuit_normal_vs = rootPath + str_Nanosuit_Normal_vs;
	string nanosuit_normal_fs = rootPath + str_Nanosuit_Normal_fs;
	string nanosuit_normal_gs = rootPath + str_Nanosuit_Normal_gs;
	Shader nanosuitNormalShader(nanosuit_normal_vs.c_str(),
		nanosuit_normal_fs.c_str(), nanosuit_normal_gs.c_str());
	if (!nanosuitNormalShader.IsValid()) {
		cout << "nanosuitNormalShader is not Valid\n";
		return 1;
	}


	//------------ 实例化着色器
	string instance_vs = rootPath + str_Instance_vs;
	string instance_fs = rootPath + str_Instance_fs;
	Shader instanceShader(instance_vs.c_str(), instance_fs.c_str());
	if (!instanceShader.IsValid()) {
		cout << "instanceShader is not Valid\n";
		return 1;
	}
	instanceShader.SetInt("texture0", 0);


	//------------ 光照着色器
	string lighting_vs = rootPath + str_Lighting_vs;
	string lighting_fs = rootPath + str_Lighting_fs;
	Shader lightingShader(lighting_vs.c_str(), lighting_fs.c_str());
	if (!lightingShader.IsValid()) {
		cout << "lightingShader is not Valid\n";
		return 1;
	} 
	
	//材质 
	lightingShader.SetInt("material.diffuse", 0);
	lightingShader.SetInt("material.specular", 1);
	lightingShader.SetInt("material.emission", 2);
	lightingShader.SetFloat("material.shininess", 32.0f);
	lightingShader.SetFloat("material.constant", 1.0f);
	lightingShader.SetFloat("material.linear", 0.09f);
	lightingShader.SetFloat("material.quadratic", 0.032f);

	nanosuitShader.SetFloat("material.shininess", 32.0f);
	//光源
	Shader * pShaders[2] = { &lightingShader, &nanosuitShader };
	for (size_t i = 0; i < 2; i++) {
		pShaders[i]->Use();
		// directional light
		pShaders[i]->SetVec3f("dirLight.direction", -0.2f, -1.0f, -0.3f);
		pShaders[i]->SetVec3f("dirLight.ambient", 0.5f, 0.5f, 0.5f);
		pShaders[i]->SetVec3f("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		pShaders[i]->SetVec3f("dirLight.specular", 0.5f, 0.5f, 0.5f);
		// point light 1
		pShaders[i]->SetVec3f("pointLights[0].position", pointLightPositions[0]);
		pShaders[i]->SetVec3f("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		pShaders[i]->SetVec3f("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		pShaders[i]->SetVec3f("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		pShaders[i]->SetFloat("pointLights[0].constant", 1.0f);
		pShaders[i]->SetFloat("pointLights[0].linear", 0.09);
		pShaders[i]->SetFloat("pointLights[0].quadratic", 0.032);
		// point light 2
		pShaders[i]->SetVec3f("pointLights[1].position", pointLightPositions[1]);
		pShaders[i]->SetVec3f("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		pShaders[i]->SetVec3f("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		pShaders[i]->SetVec3f("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		pShaders[i]->SetFloat("pointLights[1].constant", 1.0f);
		pShaders[i]->SetFloat("pointLights[1].linear", 0.09);
		pShaders[i]->SetFloat("pointLights[1].quadratic", 0.032);
		// point light 3
		pShaders[i]->SetVec3f("pointLights[2].position", pointLightPositions[2]);
		pShaders[i]->SetVec3f("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		pShaders[i]->SetVec3f("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		pShaders[i]->SetVec3f("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		pShaders[i]->SetFloat("pointLights[2].constant", 1.0f);
		pShaders[i]->SetFloat("pointLights[2].linear", 0.09);
		pShaders[i]->SetFloat("pointLights[2].quadratic", 0.032);
		// point light 4
		pShaders[i]->SetVec3f("pointLights[3].position", pointLightPositions[3]);
		pShaders[i]->SetVec3f("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		pShaders[i]->SetVec3f("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		pShaders[i]->SetVec3f("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		pShaders[i]->SetFloat("pointLights[3].constant", 1.0f);
		pShaders[i]->SetFloat("pointLights[3].linear", 0.09);
		pShaders[i]->SetFloat("pointLights[3].quadratic", 0.032);
		// spotLight
		pShaders[i]->SetVec3f("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		pShaders[i]->SetVec3f("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		pShaders[i]->SetVec3f("spotLight.specular", 1.0f, 1.0f, 1.0f);
		pShaders[i]->SetFloat("spotLight.constant", 1.0f);
		pShaders[i]->SetFloat("spotLight.linear", 0.09);
		pShaders[i]->SetFloat("spotLight.quadratic", 0.032);
		pShaders[i]->SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		pShaders[i]->SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
	}
	  
	//------------ Camera Matrixs Uniform Block
	size_t cameraMatrixsUBO;
	glGenBuffers(1, &cameraMatrixsUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, cameraMatrixsUBO);
	glBufferData(GL_UNIFORM_BUFFER, 2*sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, cameraMatrixsUBO);

	lightShader.UniformBlockBind("CameraMatrixs", 0);
	lightingShader.UniformBlockBind("CameraMatrixs", 0);
	nanosuitShader.UniformBlockBind("CameraMatrixs", 0);
	reflectShader.UniformBlockBind("CameraMatrixs", 0);
	refractShader.UniformBlockBind("CameraMatrixs", 0);
	singleColorShader.UniformBlockBind("CameraMatrixs", 0);
	skyboxShader.UniformBlockBind("CameraMatrixs", 0);
	transparentShader.UniformBlockBind("CameraMatrixs", 0);
	nanosuitExplodeShader.UniformBlockBind("CameraMatrixs", 0);
	nanosuitNormalShader.UniformBlockBind("CameraMatrixs", 0);
	instanceShader.UniformBlockBind("CameraMatrixs", 0);
	  
	//------------ nanosuit
	Model nanosuit(rootPath + str_Obj_Nanosuit);

	//------------ 帧缓存
	const float samples = 4;
	size_t FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	size_t offScreanColorBuffer;
	glGenTextures(1, &offScreanColorBuffer);
	
	//glBindTexture(GL_TEXTURE_2D, offScreanColorBuffer);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, offScreanColorBuffer);
	
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, val_windowWidth, val_windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, val_windowWidth, val_windowHeight, GL_TRUE);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, offScreanColorBuffer, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, offScreanColorBuffer, 0);

	size_t RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, val_windowWidth, val_windowHeight);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, val_windowWidth, val_windowHeight);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		printf("Framebuffer is not complete!\n");
		return 1;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//------------ 中介缓存
	// configure second post-processing framebuffer
	unsigned int intermediateFBO;
	glGenFramebuffers(1, &intermediateFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);
	// create a color attachment texture
	unsigned int screenTexture;
	glGenTextures(1, &screenTexture);
	glBindTexture(GL_TEXTURE_2D, screenTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, val_windowWidth, val_windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);	// we only need a color buffer

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	
	//------------ 输入
	auto registerInputOp = new RegisterInput(false);
	 
	//------------- 时间
	float deltaTime = 0.0f; // 当前帧与上一帧的时间差
	GStorage<float *>::GetInstance()->Register(str_DeltaTime.c_str(), &deltaTime);
	float lastFrame = 0.0f; // 上一帧的时间
	auto timeOp = new LambdaOp([&]() {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	});

	//------------ 更新相机
	auto cameraMatrixsUBO_Update = new LambdaOp([&]() {
		glBindBuffer(GL_UNIFORM_BUFFER, cameraMatrixsUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(mainCamera.GetViewMatrix()));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(mainCamera.GetProjectionMatrix()));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	});
	
	//------------ 模型渲染

	auto panelOp = new LambdaOp([&]() {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID[4]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, 0);
		lightingShader.Use();

		lightingShader.SetVec3f("viewPos", mainCamera.GetPos());
		lightingShader.SetVec3f("spotLight.position", mainCamera.GetPos());
		lightingShader.SetVec3f("spotLight.direction", mainCamera.GetFront());

		lightingShader.SetMat4f("model", glm::mat4(1.0f));
		
		glBindVertexArray(panelVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	});

	auto cubeOp = new LambdaOp([&]() {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureID[2]);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, textureID[3]);

		lightingShader.Use();

		lightingShader.SetVec3f("viewPos", mainCamera.GetPos());
		lightingShader.SetVec3f("spotLight.position", mainCamera.GetPos());
		lightingShader.SetVec3f("spotLight.direction", mainCamera.GetFront());


		singleColorShader.Use();

		glEnable(GL_CULL_FACE);

		glBindVertexArray(cubeVAO);
		glEnable(GL_STENCIL_TEST);
		for (size_t i = 0; i < 10; i++)
		{
			glCullFace( i%2==0? GL_FRONT:GL_BACK);

			glm::mat4 model(1.0f);
			float t = glfwGetTime();
			model = glm::translate(model, cubePositions[i]);

			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			model = glm::rotate(model, 1.4f * t, glm::vec3(1.0f, 1.0f, 1.0f));

			glm::mat4 border = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)*(sinf(t)*0.5f) + 1.1f);
			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)*(sinf(t)*0.5f) + 1.0f);

			lightingShader.Use();
			lightingShader.SetMat4f("model", model);

			glEnable(GL_DEPTH_TEST);
			glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
			glStencilFunc(GL_ALWAYS, 1, 0xFF); // 所有的片段都应该更新模板缓冲
			glStencilMask(0xFF); // 启用模板缓冲写入

			glDrawElements(GL_TRIANGLES, cube.GetTriNum() * 3, GL_UNSIGNED_INT, NULL);


			singleColorShader.Use();
			singleColorShader.SetMat4f("model", border);

			glStencilOp(GL_KEEP, GL_ZERO, GL_ZERO);
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0xFF);
			glDisable(GL_DEPTH_TEST);

			glDrawElements(GL_TRIANGLES, cube.GetTriNum() * 3, GL_UNSIGNED_INT, NULL);
		}

		glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilMask(0xFF); // 启用模板缓冲写入
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
	});

	auto sphereOp = new LambdaOp([&]() {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID[1]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureID[1]);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, textureID[2]);

		lightingShader.Use();

		lightingShader.SetVec3f("viewPos", mainCamera.GetPos());
		lightingShader.SetVec3f("spotLight.position", mainCamera.GetPos());
		lightingShader.SetVec3f("spotLight.direction", mainCamera.GetFront());

		glm::mat4 model = glm::mat4(1.0f);
		float t = glfwGetTime();
		model = glm::translate(model, glm::vec3(4.0f, -0.5f + sinf(2*t), -1.5f));
		lightingShader.SetMat4f("model", model);

		glBindVertexArray(sphereVAO);
		glDrawElements(GL_TRIANGLES, sphere.GetTriNum() * 3, GL_UNSIGNED_INT, NULL);
	});

	auto nanosuitOp = new LambdaOp([&]() {
		nanosuitShader.Use();

		nanosuitShader.SetVec3f("viewPos", mainCamera.GetPos());
		nanosuitShader.SetVec3f("spotLight.position", mainCamera.GetPos());
		nanosuitShader.SetVec3f("spotLight.direction", mainCamera.GetFront());

		for (size_t i = 0; i < 10; i++)
		{
			glm::mat4 model(1.0f);
			float t = glfwGetTime();
			model = glm::translate(model, glm::vec3(5.0f, 0.0f, -5.0f)); // translate it down so it's at the center of the scene
			model = glm::translate(model, cubePositions[i]);

			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)*(sinf(0.1f*i+t)*0.5f) + 1.0f);
			model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down

			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			model = glm::rotate(model, (1.4f+0.25f*i) * t, glm::vec3(1.0f, 1.0f, 1.0f));

			nanosuitShader.SetMat4f("model", model);

			nanosuit.Draw(nanosuitShader, "material.");
		}
	});

	auto lightOp = new LambdaOp([&]() {
		lightShader.Use();
		
		for (size_t i = 0; i < 4; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
			lightShader.SetMat4f("model", model);
			
			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	});

	auto grassOp = new LambdaOp([&]() {
		transparentShader.Use();
		glBindVertexArray(transparentVAO);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID[5]);
		
		for (size_t i = 0; i < 5; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, vegetationp[i]);
			transparentShader.SetMat4f("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	});

	auto glassOp = new LambdaOp([&]() {
		transparentShader.Use();
		glBindVertexArray(transparentVAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID[6]);

		std::map<float, glm::vec3> sorted;
		for (size_t i = 0; i < 5; i++)
		{
			glm::vec3 windowPos = vegetationp[i] + glm::vec3(-5.0f, 0, 0.2f);
			float distance = glm::length(mainCamera.GetPos() - windowPos);
			sorted[distance] = windowPos;
		}

		for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, it->second);
			transparentShader.SetMat4f("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
	});
	
	auto reflctNanosuitOp = new LambdaOp([&]() {
		reflectShader.Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture.GetID());

		reflectShader.SetVec3f("viewPos", mainCamera.GetPos());
		// view/projection transformations

		for (size_t i = 0; i < 10; i++)
		{
			glm::mat4 model(1.0f);
			float t = glfwGetTime();
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			model = glm::rotate(model, 0.2f*(1.4f + 0.2f*i) * t, glm::vec3(1.0f, 1.0f, 1.0f));

			model = glm::translate(model, glm::vec3(0.0f, 7.0f, -10.0f)); // translate it down so it's at the center of the scene
			model = glm::translate(model, cubePositions[i]);

			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)*(sinf(0.1f*i + t)*0.5f) + 1.0f);
			model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down

			reflectShader.SetMat4f("model", model);

			nanosuit.Draw(reflectShader);
		}
	});

	auto refractNanosuitOp = new LambdaOp([&]() {
		refractShader.Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture.GetID());

		refractShader.SetVec3f("viewPos", mainCamera.GetPos());
		// view/projection transformations

		for (size_t i = 0; i < 10; i++)
		{
			glm::mat4 model(1.0f);
			float t = glfwGetTime();
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			model = glm::rotate(model, 0.2f*(1.4f + 0.2f*i) * t, glm::vec3(1.0f, 1.0f, 1.0f));

			model = glm::translate(model, glm::vec3(1.25f, 7.0f, -10.0f)); // translate it down so it's at the center of the scene
			model = glm::translate(model, cubePositions[i]);

			model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f)*(sinf(0.1f*i + t)*0.5f) + 1.0f);
			model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down

			refractShader.SetMat4f("model", model);

			nanosuit.Draw(refractShader);
		}
	});

	auto houseOp = new LambdaOp([&]() {
		VAO_House.Use();
		houseShader.Use();
		glDrawArrays(GL_POINTS, 0, 4);
	});

	auto nanosuitExplodeOp = new LambdaOp([&]() { 
		nanosuitExplodeShader.Use();

		float t = glfwGetTime();
		nanosuitExplodeShader.SetFloat("time", t);
		glm::mat4 model(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -2.0f)); // translate it down so it's at the center of the scene
		nanosuitExplodeShader.SetMat4f("model", model);
		nanosuit.Draw(nanosuitExplodeShader);
	});

	auto nanosuitNormalOp = new LambdaOp([&]() {
		nanosuitNormalShader.Use();

		float t = glfwGetTime();
		nanosuitNormalShader.SetFloat("time", t);
		glm::mat4 model(1.0f);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, -2.0f)); // translate it down so it's at the center of the scene
		nanosuitNormalShader.SetMat4f("model", model);
		nanosuit.Draw(nanosuitNormalShader);
	});

	auto instanceOp = new LambdaOp([&]() {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID[1]);
		 
		instanceShader.Use();

		glm::mat4 model(1.0f);
		float t = glfwGetTime();
		model = glm::rotate(model, 0.2f * t, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
		instanceShader.SetMat4f("model", model);

		VAO_InstancedSphere.Use();
		glDrawElementsInstanced(GL_TRIANGLES, sphere.GetTriNum() * 3, GL_UNSIGNED_INT, 0, instanceNum);
	});

	auto skyboxOp = new LambdaOp([&]() {
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture.GetID());
		glBindVertexArray(VAO_Skybox);
		glDrawElements(GL_TRIANGLES, skybox.GetTriNum() * 3, GL_UNSIGNED_INT, NULL);
		glDepthFunc(GL_LESS); // set depth function back to default
	});

	auto opaqueQueue = new OpQueue();
	(*opaqueQueue) << cubeOp << panelOp << sphereOp << nanosuitOp
		<< lightOp << reflctNanosuitOp << refractNanosuitOp << houseOp
		<< nanosuitExplodeOp << nanosuitNormalOp << instanceOp
		<< skyboxOp;

	auto transparentQueue = new OpQueue();
	(*transparentQueue) << grassOp << glassOp;

	//------------ 离屏渲染
	auto offScreanOp = new OpNode(new LambdaOp([&]() {
		// bind to framebuffer and draw scene as we normally would to color texture 
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glClearStencil(0xFF);
		glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
		glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}),
		new LambdaOp([&]() {
		//now blit multisampled buffer(s) to normal colorbuffer of intermediate FBO. Image is stored in screenTexture
		glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
		glBlitFramebuffer(0, 0, val_windowWidth, val_windowHeight, 0, 0, val_windowWidth, val_windowHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// clear all relevant buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}));
	(*offScreanOp) << opaqueQueue << transparentQueue;
	
	//------------ 后期处理
	auto postProcessOp = new LambdaOp([&](){
		glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
		glDisable(GL_STENCIL_TEST);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, screenTexture);	// use the color offScreanColorBuffer texture as the texture of the quad plane
		postProcessShader.Use();
		glBindVertexArray(screanVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	});

	//------------ 渲染操作
	auto renderOp = new OpQueue;
	(*renderOp) << offScreanOp << postProcessOp;
	
	//------------- 末尾
	auto endOp = new LambdaOp([]() {
		glfwSwapBuffers(Glfw::GetInstance()->GetWindow());
		glfwPollEvents(); 
	});
	
	//------------- 整合
	auto opQueue = new OpQueue;
	(*opQueue) << registerInputOp << timeOp << cameraMatrixsUBO_Update << renderOp << endOp;
	
	//------------
	Glfw::GetInstance()->Run(opQueue);
	
	//------------
	Glfw::GetInstance()->Terminate();
	
	return 0;
}

Config * DoConfig() {
	printf("Try to read config.out\n");
	string rootPath;
	Config * config = new Config;
	
	rootPath = string(ROOT_PATH);
	printf("[1] First Try.\n");
	config->Load(rootPath + "/config/config.out");
	if (!config->IsValid()) {
		rootPath = "../../..";
		printf("[2] Second Try.\n");
		config->Load(rootPath + "/config/config.out");
		if (!config->IsValid()) {
			rootPath = ".";
			printf("[3] Third Try.\n");
			config->Load(rootPath + "/config.out");
			if (!config->IsValid()) {
				printf(
					"ERROR : Three Try All Fail\n"
					"ERROR : RootPath is not valid.\n"
					"Please change config/config.out 's value of RootPath and\n"
					"run exe in correct place( original place or same palce with config.out ).\n");
				exit(1);
			}
		}
	}

	*config->GetStrPtr("RootPath") = rootPath;
	printf("config.out read success\nRootPath is %s\n", config->GetStrPtr("RootPath")->c_str());
	GStorage<Config *>::GetInstance()->Register(str_MainCamera, config);
	return config;
}