/*
	泛光
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

class DrawSimpleTriangle_33 : public SimpleDrawTestBase
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
	GLuint VAO[RenderNum];
	// Element Buffer Object, EBO
	GLuint EBO[RenderNum];
	// Vertex Buffer Object, VBO
	GLuint VBO[RenderNum];
	// 纹理对象
	GLuint texture[RenderNum];
	// Shader 程序类
	Shader shader[RenderNum];
	// camera
	Camera* camera;

#pragma region "skybox"
	// 立方体贴图 // 天空盒
	unsigned int cubeTexture;
#pragma endregion

	GLuint screenWidth;
	GLuint screenHeight;

	// 平截头体的宽高比
	GLfloat aspect = 1;

	GLfloat lastX = 400.0f;
	GLfloat lastY = 300.0f;
	GLboolean firstMouse = true;

	bool useSpotLight = true;

	float deltaTime = 0.0f;

	int effectType = 0;

	DrawSimpleTriangle_33(GLuint screenWidth, GLuint screenHeight) {
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;
		aspect = (GLfloat)screenWidth / (GLfloat)screenHeight;
	}

	virtual void OnInitRender(GLFWwindow* window) {
		glfwSetWindowTitle(window, "DrawSimpleTriangle_33");
		camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f));
		camera->SpeedUpRatio = 10.0f;

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

		// 编译着色器
		shader[0] = Shader("../res/Shaders/lesson_21_hdr_lighting.vs", "../res/Shaders/lesson_21_hdr_lighting.fs");
		shader[1] = Shader("../res/Shaders/lesson_21_hdr.vs", "../res/Shaders/lesson_21_hdr.fs");

		shader[0].use();
		shader[0].setInt("diffuseTexture", 0);
		shader[1].use();
		shader[1].setInt("hdrBuffer", 0);

#pragma region "光源"
		shader[LightBoxIndex] = Shader("../res/Shaders/lesson_01_color_light.vs", "../res/Shaders/lesson_01_color_light.fs"); // 用于显示光源的小白块
#pragma endregion

#pragma region "skybox"
		shader[SkyboxIndex] = Shader("../res/Shaders/lesson_10_cubemaps.vs", "../res/Shaders/lesson_10_cubemaps.fs"); // 天空盒
#pragma endregion

		// 生成 VBO
		glGenBuffers(RenderNum, VBO);
		// 创建 EBO
		glGenBuffers(RenderNum, EBO);
		// 创建 VAO
		glGenVertexArrays(RenderNum, VAO);
		// 生成纹理对象
		glGenTextures(RenderNum, texture);
		// 加载的图像默认上下翻转
		stbi_set_flip_vertically_on_load(true);

#pragma region "光源"
		{ // 箱子 // 用来指示光源
			glBindBuffer(GL_ARRAY_BUFFER, VBO[LightBoxIndex]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(lightBoxVertices), lightBoxVertices, GL_STATIC_DRAW);
			// link vertex attributes
			glBindVertexArray(VAO[LightBoxIndex]);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
#pragma endregion

		stbi_set_flip_vertically_on_load(false);
		loadTexture(texture[0], "../res/Texture/wood.png", GL_REPEAT, GL_REPEAT, false);
		stbi_set_flip_vertically_on_load(true);


#pragma region "skybox"
		{ // 天空盒
			glBindVertexArray(VAO[SkyboxIndex]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[SkyboxIndex]);
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

		//glEnable(GL_CULL_FACE);

		//glEnable(GL_PROGRAM_POINT_SIZE);
		//glPointSize(10.5f);

		// 线框模式(Wireframe Mode)
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	virtual void OnPreRender(float deltaTime) {
		this->deltaTime = deltaTime;

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	virtual void OnRender() {
		char str[256];
		//std::cout << "call OnRender()" << std::endl;
		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera->Zoom, aspect, 0.1f, 100.0f);

#pragma region "skybox"
		if (true) { // skybox // 最先绘制 // 需关闭深度写入
			glm::mat4 skyboxView = glm::mat4(glm::mat3(camera->GetViewMatrix()));
			//glDisable(GL_CULL_FACE);
			glDepthMask(GL_FALSE);
			shader[SkyboxIndex].use();
			shader[SkyboxIndex].setMat4("view", skyboxView);
			shader[SkyboxIndex].setMat4("projection", projection);
			glBindVertexArray(VAO[SkyboxIndex]);
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

	}

#pragma region "光源"
	void RenderLightBox(glm::mat4 projection, glm::mat4 view, glm::mat4 model, glm::vec3 lightColor) {
		shader[LightBoxIndex].use();
		shader[LightBoxIndex].setMat4("projection", projection);
		shader[LightBoxIndex].setMat4("view", view);
		shader[LightBoxIndex].setMat4("model", model);
		shader[LightBoxIndex].setVec3("lightColor", lightColor);
		RenderCube(VAO[LightBoxIndex]);
	}
#pragma endregion

	// 绘制箱子
	void RenderCube(unsigned int vao) {
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

	virtual void OnOverRender() {
		//glDeleteVertexArrays(3, VAO);
		//glDeleteBuffers(3, VBO);
		delete camera;
		camera = NULL;
	}

	virtual void OnKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
		if (action == GLFW_PRESS)
		{
			if (key == GLFW_KEY_F) {
				useSpotLight = !useSpotLight;
			}
			if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9) {
				effectType = key - GLFW_KEY_0;
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
			camera->ProcessKeyboard(FORWARD, deltaTime, speedUp);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			camera->ProcessKeyboard(BACKWARD, deltaTime, speedUp);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			camera->ProcessKeyboard(LEFT, deltaTime, speedUp);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			camera->ProcessKeyboard(RIGHT, deltaTime, speedUp);
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			camera->ProcessKeyboard(UP, deltaTime, speedUp);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
			camera->ProcessKeyboard(DOWN, deltaTime, speedUp);
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

		camera->ProcessMouseMovement(xoffset, yoffset);
	}

	virtual void OnScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
		camera->ProcessMouseScroll(static_cast<float>(yoffset));
	}

	virtual void OnFrameBufferSizeChanged(GLFWwindow* window, int width, int height) {
		this->screenWidth = width;
		this->screenHeight = height;
		aspect = (GLfloat)width / (GLfloat)height;
	};
};
