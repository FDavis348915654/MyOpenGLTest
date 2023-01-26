/*
	SSAO
*/
#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <random>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

//#include <ft2build.h>
//#include FT_FREETYPE_H

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

#include <ft2build.h>
#include FT_FREETYPE_H

// 加载图片
unsigned int loadTexture(unsigned int textureID, char const * path, GLint textureWrapS, GLint textureWrapT, bool isSRGB);
// 加载天空盒
unsigned int loadCubemap(std::vector<std::string> faces);
float lerp(float a, float b, float f);

class DrawSimpleTriangle_36 : public SimpleDrawTestBase
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
	// Camera
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
	//FT_Library ft;
	//FT_Face face;
#pragma endregion

	DrawSimpleTriangle_36(GLuint screenWidth, GLuint screenHeight) {
		this->SCR_WIDTH = screenWidth;
		this->SCR_HEIGHT = screenHeight;
		aspect = (GLfloat)screenWidth / (GLfloat)screenHeight;
	}

	virtual void OnInitRender(GLFWwindow* window) {
		glfwSetWindowTitle(window, "DrawSimpleTriangle_36");
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

		{ // 文本渲染
			FT_Library ft;
			if (FT_Init_FreeType(&ft)) {
				std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
			}

			FT_Face face;
			if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face)) {
				std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
			}

			FT_Set_Pixel_Sizes(face, 0, 48);

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

		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// MSAA
		//glEnable(GL_MULTISAMPLE);
		//glDisable(GL_MULTISAMPLE);

		glEnable(GL_CULL_FACE);
		//glDisable(GL_CULL_FACE);

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
		glm::mat4 projection = glm::perspective(pCamera->Zoom, aspect, 0.1f, 50.0f);
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

		// 文本渲染
		if (true) {
			
		}
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
