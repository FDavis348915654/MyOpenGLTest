/*
	延迟着色法
*/
#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <map>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Model.h"
#include "stb_image.h"
#include "SimpleDrawTestBase.h"
#include "Camera.h"

// 加载图片
unsigned int loadTexture(unsigned int textureID, char const * path, GLint textureWrapS, GLint textureWrapT, bool isSRGB);
// 加载天空盒
unsigned int loadCubemap(std::vector<std::string> faces);

class DrawSimpleTriangle_34 : public SimpleDrawTestBase
{
public:
	// 渲染对象的数量
	static const int RenderNum = 20;

#pragma region "skybox"
	// 天空盒的索引
	static const int SkyboxIndex = RenderNum - 1;
#pragma endregion

#pragma region "光源"
	// 测试光源的索引
	static const int LightBoxIndex = RenderNum - 2;
#pragma endregion

	// Vertex Array Object, VAO
	GLuint VAOs[RenderNum];
	// Element Buffer Object, EBO
	GLuint EBOs[RenderNum];
	// Vertex Buffer Object, VBO
	GLuint VBOs[RenderNum];
	// 纹理对象
	GLuint textures[RenderNum];
	// Shader 程序类
	Shader shaders[RenderNum];
	// camera
	Camera* pCamera;

#pragma region "skybox"
	// 立方体贴图 // 天空盒
	unsigned int cubeTexture;
#pragma endregion

	GLuint SCR_WIDTH = 800;
	GLuint SCR_HEIGHT = 600;
	// 平截头体的宽高比 // (float)SCR_WIDTH / (float)SCR_HEIGHT
	GLfloat aspect = 1.0f;

	GLfloat lastX = 400.0f;
	GLfloat lastY = 300.0f;
	GLboolean firstMouse = true;

	bool useSpotLight = true;
	int effectType = 0;
	// 每一帧的间隔时间
	float deltaTime = 0.0f;

#pragma region "这节课用到的变量"
	Shader shaderGeometryPass;
	Shader shaderLightingPass;
	Shader shaderLightBox;
	Shader shaderFboDebug;

	Model backpack;
	std::vector<glm::vec3> objectPositions;

	unsigned int gBuffer;
	unsigned int gPosition, gNormal, gAlbedoSpec;

	const unsigned int NR_LIGHTS = 32;
	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColors;

	// renderCube() renders a 1x1 3D cube in NDC.
	// -------------------------------------------------
	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;

	// renderQuad() renders a 1x1 XY quad in NDC
	// -----------------------------------------
	unsigned int quadVAO = 0;
	unsigned int quadVBO;
#pragma endregion

	DrawSimpleTriangle_34(GLuint screenWidth, GLuint screenHeight) {
		this->SCR_WIDTH = screenWidth;
		this->SCR_HEIGHT = screenHeight;
		aspect = (GLfloat)screenWidth / (GLfloat)screenHeight;
	}

	virtual void OnInitRender(GLFWwindow* window) {
		glfwSetWindowTitle(window, "DrawSimpleTriangle_34");
		pCamera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f));
		pCamera->SpeedUpRatio = 10.0f;

		// 生成 VBO
		glGenBuffers(RenderNum, VBOs);
		// 创建 EBO
		glGenBuffers(RenderNum, EBOs);
		// 创建 VAO
		glGenVertexArrays(RenderNum, VAOs);
		// 生成纹理对象
		glGenTextures(RenderNum, textures);
		// 加载的图像默认上下翻转
		stbi_set_flip_vertically_on_load(true);

#pragma region "光源"
		// 箱子 // VAO[LightBoxIndex] // 用来指示光源
		float lightBoxVertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f, // bottom-left
			 1.0f,  1.0f, -1.0f, // top-right
			 1.0f, -1.0f, -1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f, // top-right
			-1.0f, -1.0f, -1.0f, // bottom-left
			-1.0f,  1.0f, -1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f, // bottom-left
			 1.0f, -1.0f,  1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f, // top-right
			 1.0f,  1.0f,  1.0f, // top-right
			-1.0f,  1.0f,  1.0f, // top-left
			-1.0f, -1.0f,  1.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, // top-right
			-1.0f,  1.0f, -1.0f, // top-left
			-1.0f, -1.0f, -1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f, // top-left
			 1.0f, -1.0f, -1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f, // top-right
			 1.0f, -1.0f, -1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f, // top-left
			 1.0f, -1.0f,  1.0f, // bottom-left
			// bottom face
			-1.0f, -1.0f, -1.0f, // top-right
			 1.0f, -1.0f, -1.0f, // top-left
			 1.0f, -1.0f,  1.0f, // bottom-left
			 1.0f, -1.0f,  1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, // bottom-right
			-1.0f, -1.0f, -1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f, // top-left
			 1.0f,  1.0f , 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f, // top-right
			 1.0f,  1.0f,  1.0f, // bottom-right
			-1.0f,  1.0f, -1.0f, // top-left
			-1.0f,  1.0f,  1.0f, // bottom-left
		};
#pragma endregion

#pragma region "skybox"
		float skyboxVertices[] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};
#pragma endregion

		{
			stbi_set_flip_vertically_on_load(false);
			// 编译着色器
			shaderGeometryPass = Shader("../res/Shaders/lesson_23_deferred_g_buffer.vs", "../res/Shaders/lesson_23_deferred_g_buffer.fs");
			shaderLightingPass = Shader("../res/Shaders/lesson_23_deferred.vs", "../res/Shaders/lesson_23_deferred.fs");
			shaderLightBox = Shader("../res/Shaders/lesson_23_deferred_light_box.vs", "../res/Shaders/lesson_23_deferred_light_box.fs");
			shaderFboDebug = Shader("../res/Shaders/lesson_23_deferred_fbo_debug.vs", "../res/Shaders/lesson_23_deferred_fbo_debug.fs");

			// load models
			backpack = Model("../res/backpack/backpack.obj");
			//backpack = Model("../res/nanosuit_reflection/nanosuit.obj");
			objectPositions.push_back(glm::vec3(-3.0, -0.5, -3.0));
			objectPositions.push_back(glm::vec3(0.0, -0.5, -3.0));
			objectPositions.push_back(glm::vec3(3.0, -0.5, -3.0));
			objectPositions.push_back(glm::vec3(-3.0, -0.5, 0.0));
			objectPositions.push_back(glm::vec3(0.0, -0.5, 0.0));
			objectPositions.push_back(glm::vec3(3.0, -0.5, 0.0));
			objectPositions.push_back(glm::vec3(-3.0, -0.5, 3.0));
			objectPositions.push_back(glm::vec3(0.0, -0.5, 3.0));
			objectPositions.push_back(glm::vec3(3.0, -0.5, 3.0));

			// configure g-buffer framebuffer
			// ------------------------------
			glGenFramebuffers(1, &gBuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
			// position color buffer
			glGenTextures(1, &gPosition);
			glBindTexture(GL_TEXTURE_2D, gPosition);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
			// normal color buffer
			glGenTextures(1, &gNormal);
			glBindTexture(GL_TEXTURE_2D, gNormal);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
			// color + specular color buffer
			glGenTextures(1, &gAlbedoSpec);
			glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);
			// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
			unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
			glDrawBuffers(3, attachments);
			// create and attach depth buffer (renderbuffer)
			unsigned int rboDepth;
			glGenRenderbuffers(1, &rboDepth);
			glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
			// finally check if framebuffer is complete
			unsigned int frameRet = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (frameRet != GL_FRAMEBUFFER_COMPLETE) {
				std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete! " << std::hex << frameRet << std::endl;
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			// lighting info
			// -------------
			srand(13);
			for (unsigned int i = 0; i < NR_LIGHTS; i++)
			{
				// calculate slightly random offsets
				float xPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
				float yPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 4.0);
				float zPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
				lightPositions.push_back(glm::vec3(xPos, yPos, zPos));
				// also calculate random color
				float rColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
				float gColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
				float bColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
				lightColors.push_back(glm::vec3(rColor, gColor, bColor));
			}
		}

#pragma region "光源"
		shaders[LightBoxIndex] = Shader("../res/Shaders/lesson_01_color_light.vs", "../res/Shaders/lesson_01_color_light.fs"); // 用于显示光源的小白块
#pragma endregion

#pragma region "skybox"
		shaders[SkyboxIndex] = Shader("../res/Shaders/lesson_10_cubemaps.vs", "../res/Shaders/lesson_10_cubemaps.fs"); // 天空盒
#pragma endregion

#pragma region "光源"
		{ // 箱子 // 用来指示光源
			glBindBuffer(GL_ARRAY_BUFFER, VBOs[LightBoxIndex]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(lightBoxVertices), lightBoxVertices, GL_STATIC_DRAW);
			// link vertex attributes
			glBindVertexArray(VAOs[LightBoxIndex]);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
#pragma endregion

#pragma region "skybox"
		{ // 天空盒
			glBindVertexArray(VAOs[SkyboxIndex]);
			glBindBuffer(GL_ARRAY_BUFFER, VBOs[SkyboxIndex]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glBindVertexArray(0);
		}
#pragma endregion

#pragma region "skybox"
		// 天空盒
		stbi_set_flip_vertically_on_load(false);
		std::vector<std::string> faces = {
			"../res/Texture/skybox/right.jpg",
			"../res/Texture/skybox/left.jpg",
			"../res/Texture/skybox/top.jpg",
			"../res/Texture/skybox/bottom.jpg",
			"../res/Texture/skybox/front.jpg",
			"../res/Texture/skybox/back.jpg"
		};
		cubeTexture = loadCubemap(faces);
		stbi_set_flip_vertically_on_load(true);
#pragma endregion

		// 开启深度测试
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// MSAA
		//glEnable(GL_MULTISAMPLE);
		//glDisable(GL_MULTISAMPLE);

		glEnable(GL_CULL_FACE);

		//glEnable(GL_PROGRAM_POINT_SIZE);
		//glPointSize(10.5f);

		// 线框模式(Wireframe Mode)
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	virtual void OnPreRender(float deltaTime) {
		this->deltaTime = deltaTime;

		//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	virtual void OnRender() {
		char str[256];
		//std::cout << "call OnRender()" << std::endl;
		glm::mat4 projection = glm::perspective(pCamera->Zoom, aspect, 0.1f, 100.0f);
		glm::mat4 view = pCamera->GetViewMatrix();

#pragma region "skybox"
		if (false) { // skybox // 最先绘制 // 需关闭深度写入
			glm::mat4 skyboxView = glm::mat4(glm::mat3(pCamera->GetViewMatrix()));
			//glDisable(GL_CULL_FACE);
			glDepthMask(GL_FALSE);
			shaders[SkyboxIndex].use();
			shaders[SkyboxIndex].setMat4("view", skyboxView);
			shaders[SkyboxIndex].setMat4("projection", projection);
			glBindVertexArray(VAOs[SkyboxIndex]);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glDepthMask(GL_TRUE);
			//glEnable(GL_CULL_FACE);
		}
#pragma endregion

#pragma region "光源"
		// 用于定位的光源
		if (false) {
			glm::vec3 lightPos(5.5f, 1.0f, 6.0f);
			// lightPos
			glm::vec3 lightColor(1.0f, 1.0f, 1.0f); // 光源颜色
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, lightPos);
			model = glm::scale(model, glm::vec3(0.05));
			RenderLightBox(projection, view, model, lightColor);
		}
#pragma endregion

		// 延迟着色法(不含光体积)
		if (true) {
			glm::mat4 model = glm::mat4(1.0f);

			{
				// 1. geometry pass: render scene's geometry/color data into gbuffer
				// -----------------------------------------------------------------
				glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				// shaderGeometryPass: lesson_23_deferred_g_buffer.vs/.fs
				shaderGeometryPass.use();
				shaderGeometryPass.setMat4("projection", projection);
				shaderGeometryPass.setMat4("view", view);
				for (unsigned int i = 0; i < objectPositions.size(); i++)
				{
					model = glm::mat4(1.0f);
					model = glm::translate(model, objectPositions[i]);
					model = glm::scale(model, glm::vec3(0.5f));
					shaderGeometryPass.setMat4("model", model);
					backpack.Draw(shaderGeometryPass);
				}
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}

			//{
			//	// 2. lighting pass: calculate lighting by iterating over a screen filled quad pixel-by-pixel using the gbuffer's content.
			//	// shaderLightingPass: lesson_23_deferred.vs/.fs
			//	// -----------------------------------------------------------------------------------------------------------------------
			//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//	shaderLightingPass.use();
			//	glActiveTexture(GL_TEXTURE0);
			//	glBindTexture(GL_TEXTURE_2D, gPosition);
			//	glActiveTexture(GL_TEXTURE1);
			//	glBindTexture(GL_TEXTURE_2D, gNormal);
			//	glActiveTexture(GL_TEXTURE2);
			//	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
			//	// shader configuration
			//	// --------------------
			//	//shaderLightingPass.use();
			//	shaderLightingPass.setInt("gPosition", 0);
			//	shaderLightingPass.setInt("gNormal", 1);
			//	shaderLightingPass.setInt("gAlbedoSpec", 2);
			//	// send light relevant uniforms
			//	for (unsigned int i = 0; i < lightPositions.size(); i++)
			//	{
			//		shaderLightingPass.setVec3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
			//		shaderLightingPass.setVec3("lights[" + std::to_string(i) + "].Color", lightColors[i]);
			//		// update attenuation parameters and calculate radius
			//		const float linear = 0.7f;
			//		const float quadratic = 1.8f;
			//		shaderLightingPass.setFloat("lights[" + std::to_string(i) + "].Linear", linear);
			//		shaderLightingPass.setFloat("lights[" + std::to_string(i) + "].Quadratic", quadratic);
			//	}
			//	shaderLightingPass.setVec3("viewPos", pCamera->Position);
			//	// finally render quad
			//	renderQuad();
			//}

			{ // fbo debug
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				shaderFboDebug.use();
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, gPosition);
				//glBindTexture(GL_TEXTURE_2D, gNormal);
				//glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
				shaderFboDebug.setInt("fboAttachment", 0);
				renderQuad();
			}

			//{
			//	// 2.5. copy content of geometry's depth buffer to default framebuffer's depth buffer
			//	// ----------------------------------------------------------------------------------
			//	glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
			//	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
			//	// blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
			//	// the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the 		
			//	// depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
			//	glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
			//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
			//}

			//{
			//	// 3. render lights on top of scene
			//	// shaderLightBox: lesson_23_deferred_light_box.vs/.fs
			//	// --------------------------------
			//	shaderLightBox.use();
			//	shaderLightBox.setMat4("projection", projection);
			//	shaderLightBox.setMat4("view", view);
			//	for (unsigned int i = 0; i < lightPositions.size(); i++)
			//	{
			//		model = glm::mat4(1.0f);
			//		model = glm::translate(model, lightPositions[i]);
			//		model = glm::scale(model, glm::vec3(0.125f));
			//		shaderLightBox.setMat4("model", model);
			//		shaderLightBox.setVec3("lightColor", lightColors[i]);
			//		renderCube();
			//	}
			//}
		}
	}

	// renderCube() renders a 1x1 3D cube in NDC.
	// -------------------------------------------------
	void renderCube()
	{
		// initialize (if necessary)
		if (cubeVAO == 0)
		{
			float vertices[] = {
				// back face
				-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
				 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
				 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
				 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
				-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
				-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
				// front face
				-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
				 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
				 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
				 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
				-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
				-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
				// left face
				-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
				-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
				-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
				-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
				-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
				-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
				// right face
				 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
				 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
				 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
				 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
				 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
				 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
				// bottom face
				-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
				 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
				 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
				 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
				-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
				-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
				// top face
				-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
				 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
				 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
				 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
				-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
				-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
			};
			glGenVertexArrays(1, &cubeVAO);
			glGenBuffers(1, &cubeVBO);
			// fill buffer
			glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			// link vertex attributes
			glBindVertexArray(cubeVAO);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
		// render Cube
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

	// renderQuad() renders a 1x1 XY quad in NDC
	// -----------------------------------------
	void renderQuad()
	{
		if (quadVAO == 0)
		{
			float quadVertices[] = {
				// positions        // texture Coords
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};
			// setup plane VAO
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		}
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

#pragma region "光源"
	void RenderLightBox(glm::mat4 projection, glm::mat4 view, glm::mat4 model, glm::vec3 lightColor) {
		shaders[LightBoxIndex].use();
		shaders[LightBoxIndex].setMat4("projection", projection);
		shaders[LightBoxIndex].setMat4("view", view);
		shaders[LightBoxIndex].setMat4("model", model);
		shaders[LightBoxIndex].setVec3("lightColor", lightColor);
		RenderCube(VAOs[LightBoxIndex]);
	}

	// 绘制箱子
	void RenderCube(unsigned int vao) {
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}
#pragma endregion

	virtual void OnOverRender() {
		//glDeleteVertexArrays(RenderNum, VAOs);
		//glDeleteBuffers(RenderNum, VBOs);
		delete pCamera;
		pCamera = NULL;
	}

	virtual void OnKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
		if (action == GLFW_PRESS)
		{
			if (key == GLFW_KEY_F) {
				useSpotLight = !useSpotLight;
				std::cout << "OnKeyCallback, useSpotLight:" << useSpotLight << std::endl;
			}
			if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9) {
				effectType = key - GLFW_KEY_0;
				std::cout << "OnKeyCallback, effectType:" << effectType << std::endl;
			}
		}
	}

	virtual void OnProcessInput(GLFWwindow* window) {
		bool speedUp = false;
		// 加速
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			speedUp = true;
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			pCamera->ProcessKeyboard(FORWARD, deltaTime, speedUp);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			pCamera->ProcessKeyboard(BACKWARD, deltaTime, speedUp);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			pCamera->ProcessKeyboard(LEFT, deltaTime, speedUp);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			pCamera->ProcessKeyboard(RIGHT, deltaTime, speedUp);
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			pCamera->ProcessKeyboard(UP, deltaTime, speedUp);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
			pCamera->ProcessKeyboard(DOWN, deltaTime, speedUp);
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			//std::cout << "OnProcessInput, exposure:" << exposure << std::endl;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			//std::cout << "OnProcessInput, exposure:" << exposure << std::endl;
		}
	}

	virtual void OnMouseCallback(GLFWwindow* window, double xpos, double ypos) {
		//printf("screenPos(%f, %f)\n", xpos, ypos);
		if (firstMouse) {
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
			return;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
		lastX = xpos;
		lastY = ypos;

		pCamera->ProcessMouseMovement(xoffset, yoffset);
	}

	virtual void OnScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
		pCamera->ProcessMouseScroll(static_cast<float>(yoffset));
	}

	virtual void OnFrameBufferSizeChanged(GLFWwindow* window, int width, int height) {
		this->SCR_WIDTH = width;
		this->SCR_HEIGHT = height;
		aspect = (GLfloat)width / (GLfloat)height;
	};
};
