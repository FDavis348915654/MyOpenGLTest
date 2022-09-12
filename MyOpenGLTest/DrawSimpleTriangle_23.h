/*
	几何着色器
*/
#pragma once
#include <string>
#include <iostream>
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
unsigned int loadTexture(unsigned int textureID, char const * path, GLint textureWrapS, GLint textureWrapT);
// 加载天空盒
unsigned int loadCubemap(std::vector<std::string> faces);

class DrawSimpleTriangle_23 : public SimpleDrawTestBase
{
public:
	// Vertex Array Object, VAO
	GLuint VAO[10];
	// Element Buffer Object, EBO
	GLuint EBO[10];
	// Vertex Buffer Object, VBO
	GLuint VBO[10];
	// 纹理对象
	GLuint texture[10];
	// Shader 程序类
	Shader shader[10];
	// camera
	Camera* camera;
	// Framebuffer Object // 帧缓冲对象
	unsigned int fbo;
	// Renderbuffer Object // 渲染缓冲对象/渲染附件
	unsigned int rbo;
	// 纹理附件
	unsigned int texColorBuffer;
	// 立方体贴图
	unsigned int cubeTexture;
	// uniform buffer object
	unsigned int uboMatrices;

	Model ourModel;

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

	DrawSimpleTriangle_23(GLuint screenWidth, GLuint screenHeight) {
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;
		aspect = (GLfloat)screenWidth / (GLfloat)screenHeight;
	}

	virtual void OnInitRender(GLFWwindow* window) {
		glfwSetWindowTitle(window, "DrawSimpleTriangle_23");
		camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
		//float points[] = {
		//	-0.5f,  0.5f, // 左上
		//	 0.5f,  0.5f, // 右上
		//	 0.5f, -0.5f, // 右下
		//	-0.5f, -0.5f  // 左下
		//};
		float points[] = {
			-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // 左上
			 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // 右上
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // 右下
			-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // 左下
		};
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

		// 编译着色器
		shader[0] = Shader("../res/Shaders/lesson_12_geometry.vs", "../res/Shaders/lesson_12_geometry.gs", "../res/Shaders/lesson_12_geometry.frag"); // 基础
		//shader[0] = Shader("../res/Shaders/lesson_12_geometry.vs", "../res/Shaders/lesson_12_geometry_test.gs", "../res/Shaders/lesson_12_geometry.frag"); // 把点拉长
		shader[1] = Shader("../res/Shaders/lesson_12_geometry.vs", "../res/Shaders/lesson_12_geometry_build_house.gs", "../res/Shaders/lesson_12_geometry.frag"); // 基础房子
		shader[2] = Shader("../res/Shaders/lesson_12_geometry_build_house_color.vs", "../res/Shaders/lesson_12_geometry_build_house_color.gs", "../res/Shaders/lesson_12_geometry_build_house_color.frag"); // 带颜色房子
		shader[3] = Shader("../res/Shaders/lesson_10_cubemaps.vs", "../res/Shaders/lesson_10_cubemaps.frag"); // 天空盒
		shader[4] = Shader("../res/Shaders/lesson_12_geometry_explode.vs", "../res/Shaders/lesson_12_geometry_explode.gs", "../res/Shaders/lesson_12_geometry_explode.frag"); // 模型 爆炸效果
		shader[5] = Shader("../res/Shaders/lesson_10_cubemaps_4.vs", "../res/Shaders/lesson_10_cubemaps_4_test.frag"); // 模型 只绘制了漫反射纹理
		shader[6] = Shader("../res/Shaders/lesson_12_geometry_fur.vs", "../res/Shaders/lesson_12_geometry_fur.gs", "../res/Shaders/lesson_12_geometry_fur.frag"); // 模型 只绘制法线
		// 生成 VBO
		glGenBuffers(10, VBO);
		// 创建 EBO
		glGenBuffers(10, EBO);
		// 创建 VAO
		glGenVertexArrays(10, VAO);
		// 生成纹理对象
		glGenTextures(10, texture);
		// 加载的图像默认上下翻转
		stbi_set_flip_vertically_on_load(true);

		//{ // 设置顶点属性 // 四个点
		//	// Vertex Array Object
		//	glBindVertexArray(VAO[0]);
		//	// 复制顶点数组到一个顶点缓冲中供 OpenGL 使用
		//	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		//	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
		//	glEnableVertexAttribArray(0);
		//	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		//	glBindVertexArray(0);
		//}

		{ // 设置顶点属性 // 四个点(带颜色)
			// Vertex Array Object
			glBindVertexArray(VAO[0]);
			// 复制顶点数组到一个顶点缓冲中供 OpenGL 使用
			glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
			glBindVertexArray(0);
		}

		{ // 天空盒
			glBindVertexArray(VAO[3]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glBindVertexArray(0);
		}

		// 绑定一个纹理对象, 为当前绑定的纹理对象设置环绕、过滤方式 // 木箱
		loadTexture(texture[0], "../res/Texture/container2.png", GL_REPEAT, GL_REPEAT);
		// 绑定一个纹理对象, 为当前绑定的纹理对象设置环绕、过滤方式 // 高光
		loadTexture(texture[1], "../res/Texture/container2_specular.png", GL_REPEAT, GL_REPEAT);
		// 绑定一个纹理对象, 为当前绑定的纹理对象设置环绕、过滤方式 // 草丛
		loadTexture(texture[2], "../res/Texture/grass.png", GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
		// 绑定一个纹理对象, 为当前绑定的纹理对象设置环绕、过滤方式 // 玻璃窗
		loadTexture(texture[3], "../res/Texture/blending_transparent_window.png", GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

		stbi_set_flip_vertically_on_load(false);
		ourModel = Model("../res/nanosuit_reflection/nanosuit.obj");
		stbi_set_flip_vertically_on_load(true);

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

		// 开启深度测试
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

		if (true) { // skybox // 最先绘制 // 需关闭深度写入
			glm::mat4 skyboxView = glm::mat4(glm::mat3(camera->GetViewMatrix()));
			//glDisable(GL_CULL_FACE);
			glDepthMask(GL_FALSE);
			shader[3].use();
			shader[3].setMat4("view", skyboxView);
			shader[3].setMat4("projection", projection);
			glBindVertexArray(VAO[3]);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glDepthMask(GL_TRUE);
			//glEnable(GL_CULL_FACE);
		}

		// 4 个点
		if (false) {
			shader[0].use();
			glBindVertexArray(VAO[0]);
			glDrawArrays(GL_POINTS, 0, 4);
		}

		// 4 个房子
		if (false) {
			shader[1].use();
			glBindVertexArray(VAO[0]);
			glDrawArrays(GL_POINTS, 0, 4);
		}

		// 4 个房子(带颜色)
		if (false) {
			shader[2].use();
			glBindVertexArray(VAO[0]);
			glDrawArrays(GL_POINTS, 0, 4);
		}

		float modelScale = 1.0f;

		// 绘制纳米生化装, 爆炸
		if (false) {
			shader[4].use();
			shader[4].setVec3("cameraPos", camera->Position);
			shader[4].setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));
			shader[4].setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));
			glm::mat4 model;
			model = glm::translate(model, glm::vec3(0.0f * modelScale, -10.0f * modelScale, -8.0f * modelScale));
			model = glm::scale(model, glm::vec3(modelScale, modelScale, modelScale));
			shader[4].setMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));
			shader[4].setFloat("time", glfwGetTime());
			ourModel.Draw(shader[4]);
		}

		// 绘制纳米生化装, 长毛
		if (true) {
			shader[5].use();
			shader[5].setVec3("cameraPos", camera->Position);
			shader[5].setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));
			shader[5].setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));
			glm::mat4 model;
			model = glm::translate(model, glm::vec3(0.0f * modelScale, -10.0f * modelScale, -8.0f * modelScale));
			model = glm::scale(model, glm::vec3(modelScale, modelScale, modelScale));
			shader[5].setMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));
			shader[5].setFloat("time", glfwGetTime());
			ourModel.Draw(shader[5]);

			shader[6].use();
			shader[6].setVec3("cameraPos", camera->Position);
			shader[6].setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));
			shader[6].setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f * modelScale, -10.0f * modelScale, -8.0f * modelScale));
			model = glm::scale(model, glm::vec3(modelScale, modelScale, modelScale));
			shader[6].setMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));
			ourModel.Draw(shader[6]);
		}
	}

	virtual void OnOverRender() {
		/*	glDeleteVertexArrays(3, VAO);
			glDeleteBuffers(3, VBO);*/
		delete camera;
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
