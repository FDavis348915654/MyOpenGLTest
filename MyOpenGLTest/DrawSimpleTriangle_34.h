/*
	�ӳ���ɫ��
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

// ����ͼƬ
unsigned int loadTexture(unsigned int textureID, char const * path, GLint textureWrapS, GLint textureWrapT, bool isSRGB);
// ������պ�
unsigned int loadCubemap(std::vector<std::string> faces);

class DrawSimpleTriangle_34 : public SimpleDrawTestBase
{
public:
	// ��Ⱦ���������
	static const int RenderNum = 20;

#pragma region "skybox"
	// ��պе�����
	static const int SkyboxIndex = RenderNum - 1;
#pragma endregion

#pragma region "��Դ"
	// ���Թ�Դ������
	static const int LightBoxIndex = RenderNum - 2;
#pragma endregion

	// Vertex Array Object, VAO
	GLuint VAOs[RenderNum];
	// Element Buffer Object, EBO
	GLuint EBOs[RenderNum];
	// Vertex Buffer Object, VBO
	GLuint VBOs[RenderNum];
	// �������
	GLuint textures[RenderNum];
	// Shader ������
	Shader shaders[RenderNum];
	// camera
	Camera* pCamera;

#pragma region "skybox"
	// ��������ͼ // ��պ�
	unsigned int cubeTexture;
#pragma endregion

	GLuint SCR_WIDTH = 800;
	GLuint SCR_HEIGHT = 600;
	// ƽ��ͷ��Ŀ�߱� // (float)SCR_WIDTH / (float)SCR_HEIGHT
	GLfloat aspect = 1.0f;

	GLfloat lastX = 400.0f;
	GLfloat lastY = 300.0f;
	GLboolean firstMouse = true;

	bool useSpotLight = true;
	int effectType = 0;
	// ÿһ֡�ļ��ʱ��
	float deltaTime = 0.0f;

#pragma region "��ڿ��õ��ı���"
	// �õ��� shader
	Shader shader;
	Shader shaderLight;
	Shader shaderBlur;
	Shader shaderBloomFinal;
	unsigned int hdrFBO;
	unsigned int colorBuffers[2];
	unsigned int pingpongFBO[2];
	unsigned int pingpongColorbuffers[2];
	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;
	unsigned int quadVAO = 0;
	unsigned int quadVBO;
	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColors;
	unsigned int woodTexture;
	unsigned int containerTexture;

	bool bloom = true;
	float exposure = 1.0f;
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

#pragma region "��Դ"
		// ���� // VAO[LightBoxIndex] // ����ָʾ��Դ
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

		// ������ɫ��
		shaders[0] = Shader("../res/Shaders/lesson_22_bloom.vs", "../res/Shaders/lesson_22_bloom.fs");
		shaders[1] = Shader("../res/Shaders/lesson_22_bloom.vs", "../res/Shaders/lesson_22_bloom_light_box.fs");
		shaders[2] = Shader("../res/Shaders/lesson_22_bloom_blur.vs", "../res/Shaders/lesson_22_bloom_blur.fs");
		shaders[3] = Shader("../res/Shaders/lesson_22_bloom_final.vs", "../res/Shaders/lesson_22_bloom_final.fs");

		shader = shaders[0];
		shaderLight = shaders[1];
		shaderBlur = shaders[2];
		shaderBloomFinal = shaders[3];

#pragma region "��Դ"
		shaders[LightBoxIndex] = Shader("../res/Shaders/lesson_01_color_light.vs", "../res/Shaders/lesson_01_color_light.fs"); // ������ʾ��Դ��С�׿�
#pragma endregion

#pragma region "skybox"
		shaders[SkyboxIndex] = Shader("../res/Shaders/lesson_10_cubemaps.vs", "../res/Shaders/lesson_10_cubemaps.fs"); // ��պ�
#pragma endregion

		// ���� VBO
		glGenBuffers(RenderNum, VBOs);
		// ���� EBO
		glGenBuffers(RenderNum, EBOs);
		// ���� VAO
		glGenVertexArrays(RenderNum, VAOs);
		// �����������
		glGenTextures(RenderNum, textures);
		// ���ص�ͼ��Ĭ�����·�ת
		stbi_set_flip_vertically_on_load(true);

#pragma region "��Դ"
		{ // ���� // ����ָʾ��Դ
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

		stbi_set_flip_vertically_on_load(false);
		loadTexture(textures[0], "../res/Texture/wood.png", GL_REPEAT, GL_REPEAT, false);
		loadTexture(textures[1], "../res/Texture/container2.png", GL_REPEAT, GL_REPEAT, true);
		stbi_set_flip_vertically_on_load(true);

		woodTexture = textures[0];
		containerTexture = textures[1];

#pragma region "skybox"
		{ // ��պ�
			glBindVertexArray(VAOs[SkyboxIndex]);
			glBindBuffer(GL_ARRAY_BUFFER, VBOs[SkyboxIndex]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glBindVertexArray(0);
		}
#pragma endregion

#pragma region "skybox"
		// ��պ�
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

		{ // hdrFBO
			std::cout << "init hdrFBO" << std::endl;
			glGenFramebuffers(1, &hdrFBO);
			glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
			// create 2 floating point color buffers (1 for normal rendering, other for brightness threshold values)
			glGenTextures(2, colorBuffers);
			for (unsigned int i = 0; i < 2; i++)
			{
				glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				// attach texture to framebuffer
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
			}
			// create and attach depth buffer (renderbuffer)
			unsigned int rboDepth;
			glGenRenderbuffers(1, &rboDepth);
			glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
			// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
			unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
			glDrawBuffers(2, attachments);
			// finally check if framebuffer is complete
			unsigned int frameRet = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (frameRet != GL_FRAMEBUFFER_COMPLETE) {
				std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete! " << std::hex << frameRet << std::endl;
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		{ // ���ڻ���ģ���� FBO // ping-pong-framebuffer for blurring
			std::cout << "init pingpongFBO" << std::endl;
			glGenFramebuffers(2, pingpongFBO);
			glGenTextures(2, pingpongColorbuffers);
			for (unsigned int i = 0; i < 2; i++)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
				glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
				// also check if framebuffers are complete (no need for depth buffer)
				unsigned int frameRet = glCheckFramebufferStatus(GL_FRAMEBUFFER);
				if (frameRet != GL_FRAMEBUFFER_COMPLETE) {
					std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete! " << std::hex << frameRet << std::endl;
				}
			}
		}

		{ // ��Ⱦǰ���ù��ղ����� shader ����
			// lighting info
			// -------------
			// positions
			lightPositions.push_back(glm::vec3(0.0f, 0.5f, 1.5f));
			lightPositions.push_back(glm::vec3(-4.0f, 0.5f, -3.0f));
			lightPositions.push_back(glm::vec3(3.0f, 0.5f, 1.0f));
			lightPositions.push_back(glm::vec3(-.8f, 2.4f, -1.0f));
			// colors
			lightColors.push_back(glm::vec3(5.0f, 5.0f, 5.0f));
			lightColors.push_back(glm::vec3(10.0f, 0.0f, 0.0f));
			lightColors.push_back(glm::vec3(0.0f, 0.0f, 15.0f));
			lightColors.push_back(glm::vec3(0.0f, 5.0f, 0.0f));


			// shader configuration
			// --------------------
			shader.use();
			shader.setInt("diffuseTexture", 0);
			shaderBlur.use();
			shaderBlur.setInt("image", 0);
			shaderBloomFinal.use();
			shaderBloomFinal.setInt("scene", 0);
			shaderBloomFinal.setInt("bloomBlur", 1);
		}

		// ������Ȳ���
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// MSAA
		//glEnable(GL_MULTISAMPLE);
		//glDisable(GL_MULTISAMPLE);

		//glEnable(GL_CULL_FACE);

		//glEnable(GL_PROGRAM_POINT_SIZE);
		//glPointSize(10.5f);

		// �߿�ģʽ(Wireframe Mode)
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
		if (false) { // skybox // ���Ȼ��� // ��ر����д��
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

#pragma region "��Դ"
		// ���ڶ�λ�Ĺ�Դ
		if (false) {
			glm::vec3 lightPos(5.5f, 1.0f, 6.0f);
			// lightPos
			glm::vec3 lightColor(1.0f, 1.0f, 1.0f); // ��Դ��ɫ
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, lightPos);
			model = glm::scale(model, glm::vec3(0.05));
			RenderLightBox(projection, view, model, lightColor);
		}
#pragma endregion

		{
			// 1. render scene into floating point framebuffer
			// -----------------------------------------------
			glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glm::mat4 model = glm::mat4(1.0f);
			// �����Ӳ��Ѹ߹ⲿ�ֵ�Ƭ������� colorBuffers[1]
			shader.use();
			shader.setMat4("projection", projection);
			shader.setMat4("view", view);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, woodTexture);
			// set lighting uniforms
			for (unsigned int i = 0; i < lightPositions.size(); i++)
			{
				shader.setVec3("lights[" + std::to_string(i) + "].Position", lightPositions[i]);
				shader.setVec3("lights[" + std::to_string(i) + "].Color", lightColors[i]);
			}
			shader.setVec3("viewPos", pCamera->Position);
			// create one large cube that acts as the floor
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0));
			model = glm::scale(model, glm::vec3(12.5f, 0.5f, 12.5f));
			shader.setMat4("model", model);
			renderCube();
			// then create multiple cubes as the scenery
			glBindTexture(GL_TEXTURE_2D, containerTexture);
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
			model = glm::scale(model, glm::vec3(0.5f));
			shader.setMat4("model", model);
			renderCube();

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
			model = glm::scale(model, glm::vec3(0.5f));
			shader.setMat4("model", model);
			renderCube();

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 2.0));
			model = glm::rotate(model, 60.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
			shader.setMat4("model", model);
			renderCube();

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, 2.7f, 4.0));
			model = glm::rotate(model, 23.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
			model = glm::scale(model, glm::vec3(1.25));
			shader.setMat4("model", model);
			renderCube();

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -3.0));
			model = glm::rotate(model, 124.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
			shader.setMat4("model", model);
			renderCube();

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0));
			model = glm::scale(model, glm::vec3(0.5f));
			shader.setMat4("model", model);
			renderCube();

			// ����Դ���Ѹ߹ⲿ�ֵ�Ƭ������� colorBuffers[1]
			// finally show all the light sources as bright cubes
			shaderLight.use();
			shaderLight.setMat4("projection", projection);
			shaderLight.setMat4("view", view);

			for (unsigned int i = 0; i < lightPositions.size(); i++)
			{
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(lightPositions[i]));
				model = glm::scale(model, glm::vec3(0.25f));
				shaderLight.setMat4("model", model);
				shaderLight.setVec3("lightColor", lightColors[i]);
				renderCube();
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			// 2. blur bright fragments with two-pass Gaussian Blur 
			// --------------------------------------------------
			bool horizontal = true, first_iteration = true;
			unsigned int amount = 10;
			shaderBlur.use();
			for (unsigned int i = 0; i < amount; i++)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
				shaderBlur.setInt("horizontal", horizontal);
				glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
				renderQuad();
				horizontal = !horizontal;
				if (first_iteration) {
					first_iteration = false;
				}
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			// 3. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
			// --------------------------------------------------------------------------------------------------------------------------
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			shaderBloomFinal.use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);
			shaderBloomFinal.setInt("bloom", bloom);
			shaderBloomFinal.setFloat("exposure", exposure);
			renderQuad();
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

#pragma region "��Դ"
	void RenderLightBox(glm::mat4 projection, glm::mat4 view, glm::mat4 model, glm::vec3 lightColor) {
		shaders[LightBoxIndex].use();
		shaders[LightBoxIndex].setMat4("projection", projection);
		shaders[LightBoxIndex].setMat4("view", view);
		shaders[LightBoxIndex].setMat4("model", model);
		shaders[LightBoxIndex].setVec3("lightColor", lightColor);
		RenderCube(VAOs[LightBoxIndex]);
	}

	// ��������
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
				bloom = useSpotLight;
			}
			if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9) {
				effectType = key - GLFW_KEY_0;
				std::cout << "OnKeyCallback, effectType:" << effectType << std::endl;
			}
		}
	}

	virtual void OnProcessInput(GLFWwindow* window) {
		bool speedUp = false;
		// ����
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
			exposure += 0.001f;
			std::cout << "OnProcessInput, exposure:" << exposure << std::endl;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			if (exposure > 0.0f) {
				exposure -= 0.001f;
			}
			else {
				exposure = 0.0f;
			}
			std::cout << "OnProcessInput, exposure:" << exposure << std::endl;
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
		float yoffset = lastY - ypos; // ע���������෴�ģ���Ϊy�����Ǵӵײ����������������
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
