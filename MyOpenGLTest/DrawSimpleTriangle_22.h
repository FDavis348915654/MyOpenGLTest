/*
	高级数据/高级GLSL
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

class DrawSimpleTriangle_22 : public SimpleDrawTestBase
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

	DrawSimpleTriangle_22(GLuint screenWidth, GLuint screenHeight) {
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;
		aspect = (GLfloat)screenWidth / (GLfloat)screenHeight;
	}

	virtual void OnInitRender(GLFWwindow* window) {
		glfwSetWindowTitle(window, "DrawSimpleTriangle_22");
		camera = new Camera((glm::vec3(0.0f, 0.0f, 3.0f)));
		//std::cout << "call PreRender()" << std::endl;
		float cubeVertices[] = {
			// Back face
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
			// Front face
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
			// Left face
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
			// Right face
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
			// Bottom face
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
			// Top face
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left        
		};
		float planeVertices[] = {
			// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
			 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
			-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
			-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

			 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
			-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
			 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
		};
		//std::cout << "sizeof planeVertices " << sizeof(planeVertices) << std::endl; // 120
		float transparentVertices[] = {
			// positions         // texture Coords
			0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
			0.0f, -0.5f,  0.0f,  0.0f,  0.0f,
			1.0f, -0.5f,  0.0f,  1.0f,  0.0f,

			0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
			1.0f, -0.5f,  0.0f,  1.0f,  0.0f,
			1.0f,  0.5f,  0.0f,  1.0f,  1.0f
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
		float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
		};

		// 编译着色器
		shader[0] = Shader("../res/Shaders/lesson_11_advanced_GLSL.vs", "../res/Shaders/lesson_11_advanced_GLSL.frag"); // 用来画点并设置点的大小
		shader[1] = Shader("../res/Shaders/lesson_11_advanced_GLSL_uniform.vs", "../res/Shaders/lesson_11_advanced_GLSL_uniform_red.frag"); // Uniform缓冲对象
		shader[2] = Shader("../res/Shaders/lesson_11_advanced_GLSL_uniform.vs", "../res/Shaders/lesson_11_advanced_GLSL_uniform_green.frag"); // Uniform缓冲对象
		shader[3] = Shader("../res/Shaders/lesson_11_advanced_GLSL_uniform.vs", "../res/Shaders/lesson_11_advanced_GLSL_uniform_blue.frag"); // Uniform缓冲对象
		shader[4] = Shader("../res/Shaders/lesson_11_advanced_GLSL_uniform.vs", "../res/Shaders/lesson_11_advanced_GLSL_uniform_yellow.frag"); // Uniform缓冲对象
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

		{ // 设置顶点属性 // 箱子
			// Vertex Array Object
			glBindVertexArray(VAO[0]);
			// 复制顶点数组到一个顶点缓冲中供 OpenGL 使用
			glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			glBindVertexArray(0);
		}

		{ // 设置顶点属性 // 平面
			// Vertex Array Object
			glBindVertexArray(VAO[1]);
			// 复制顶点数组到一个顶点缓冲中供 OpenGL 使用
			glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			glBindVertexArray(0);
		}

		{ // 设置顶点属性 // 草丛
			// Vertex Array Object
			glBindVertexArray(VAO[2]);
			// 复制顶点数组到一个顶点缓冲中供 OpenGL 使用
			glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
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

		{ // 设置顶点属性 // 箱子 // 反射
			// Vertex Array Object
			glBindVertexArray(VAO[4]);
			// 复制顶点数组到一个顶点缓冲中供 OpenGL 使用
			glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
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
		//ourModel = Model("../res/nanosuit_reflection/nanosuit.obj");
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

		// 获取各个着色器的 Uniform块
		unsigned int uniformBlockIndexRed = glGetUniformBlockIndex(shader[1].ID, "Matrices");
		unsigned int uniformBlockIndexGreen = glGetUniformBlockIndex(shader[2].ID, "Matrices");
		unsigned int uniformBlockIndexBlue = glGetUniformBlockIndex(shader[3].ID, "Matrices");
		unsigned int uniformBlockIndexYellow = glGetUniformBlockIndex(shader[4].ID, "Matrices");
		// 将各个着色器上的 Uniform块 链接至 绑定点0
		glUniformBlockBinding(shader[1].ID, uniformBlockIndexRed, 0);
		glUniformBlockBinding(shader[2].ID, uniformBlockIndexGreen, 0);
		glUniformBlockBinding(shader[3].ID, uniformBlockIndexBlue, 0);
		glUniformBlockBinding(shader[4].ID, uniformBlockIndexYellow, 0);
		// gen ubo
		glGenBuffers(1, &uboMatrices);
		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));

		glm::mat4 projection = glm::perspective(45.0f, (float)screenWidth/(float)screenHeight, 0.1f, 100.0f);
		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);


		// 开启深度测试
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//glEnable(GL_CULL_FACE);

		glEnable(GL_PROGRAM_POINT_SIZE);
		glPointSize(10.5f);

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

		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		if (false) { // cubes // 普通的箱子
			shader[0].use();
			shader[0].setMat4("view", view);
			shader[0].setMat4("projection", projection);
			glBindVertexArray(VAO[0]);

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-1.0f, 0.001f, -1.0f));
			shader[0].setMat4("model", model);
			//glDrawArrays(GL_TRIANGLES, 0, 36);
			glDrawArrays(GL_POINTS, 0, 36);

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(2.0f, 0.001f, 0.0f));
			shader[0].setMat4("model", model);
			//glDrawArrays(GL_TRIANGLES, 0, 36);
			glDrawArrays(GL_POINTS, 0, 36);
		}

		if (true) { // uniform 缓冲对象
			glBindVertexArray(VAO[0]);
			shader[1].use();
			glm::mat4 model;
			model = glm::translate(model, glm::vec3(-0.75f, 0.75f, 0.0f));
			shader[1].setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			shader[2].use();
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.75f, 0.75f, 0.0f));
			shader[2].setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			shader[3].use();
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.75f, -0.75f, 0.0f));
			shader[3].setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);

			shader[4].use();
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-0.75f, -0.75f, 0.0f));
			shader[4].setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
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
