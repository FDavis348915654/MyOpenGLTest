/*
	投光物
*/
#pragma once
#include <string>
#include <iostream>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "stb_image.h"
#include "SimpleDrawTestBase.h"
#include "Camera.h"

// 加载图片
unsigned int loadTexture(unsigned int textureID, char const * path, GLint textureWrapS, GLint textureWrapT, bool isSRGB);

class DrawSimpleTriangle_13 : public SimpleDrawTestBase
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

	GLuint screenWidth;
	GLuint screenHeight;

	// 平截头体的宽高比
	GLfloat aspect = 1;

	GLfloat lastX = 400.0f;
	GLfloat lastY = 300.0f;
	GLboolean firstMouse = true;

	// 是否使用灯光
	bool useSpotLight = true;

	float deltaTime = 0.0f;

	DrawSimpleTriangle_13(GLuint screenWidth, GLuint screenHeight) {
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;
		aspect = (GLfloat)screenWidth / (GLfloat)screenHeight;
	}

	virtual void OnInitRender(GLFWwindow* window) {
		glfwSetWindowTitle(window, "DrawSimpleTriangle_13");
		camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
		//std::cout << "call PreRender()" << std::endl;
		float vertices[] = {
			// positions          // normals           // texture coords
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
		};

		// 编译着色器
		shader[0] = Shader("../res/Shaders/lesson_01_color_light.vs", "../res/Shaders/lesson_01_color_light.fs"); // 用于显示光源的小白块
		shader[1] = Shader("../res/Shaders/lesson_04_light_casters.vs", "../res/Shaders/lesson_04_light_casters.fs");
		shader[2] = Shader("../res/Shaders/lesson_04_light_casters.vs", "../res/Shaders/lesson_04_light_casters_direction.fs"); // 平行光
		shader[3] = Shader("../res/Shaders/lesson_04_light_casters.vs", "../res/Shaders/lesson_04_light_casters_point.fs"); // 点光源
		shader[4] = Shader("../res/Shaders/lesson_04_light_casters.vs", "../res/Shaders/lesson_04_light_casters_spotlight.fs"); // 聚光
		shader[5] = Shader("../res/Shaders/lesson_04_light_casters.vs", "../res/Shaders/lesson_04_light_casters_spotlight_soft.fs"); // 聚光软化
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

		// 绑定一个纹理对象, 为当前绑定的纹理对象设置环绕、过滤方式 // 木箱
		loadTexture(texture[0], "../res/Texture/container2.png", GL_REPEAT, GL_REPEAT, false);

		// 绑定一个纹理对象, 为当前绑定的纹理对象设置环绕、过滤方式 // 高光
		loadTexture(texture[1], "../res/Texture/container2_specular.png", GL_REPEAT, GL_REPEAT, false);

		{ // 设置顶点属性 // 灯
			// Vertex Array Object
			glBindVertexArray(VAO[0]);
			// 复制顶点数组到一个顶点缓冲中供 OpenGL 使用
			glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			// vertex
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			glBindVertexArray(0);
		}

		{ // 设置顶点属性 // Box
			// Vertex Array Object
			glBindVertexArray(VAO[1]);
			// 复制顶点数组到一个顶点缓冲中供 OpenGL 使用
			glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			// vertex
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			// uv
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);
			// texture coords
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
			glEnableVertexAttribArray(2);
			glBindVertexArray(0);
		}

		// 开启深度测试
		glEnable(GL_DEPTH_TEST);
	}

	virtual void OnPreRender(float deltaTime) {
		this->deltaTime = deltaTime;
		// Clear the colorbuffer
		if (false) {
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		// z 缓冲
		if (true) {
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
	}

	virtual void OnRender() {
		//std::cout << "call OnRender()" << std::endl;
		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera->Zoom, aspect, 0.1f, 100.0f);
		// 光源位置
		glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

		// 光源颜色
		glm::vec3 lightColor(1.0f, 1.0f, 1.0f); // 光源颜色
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // 漫反射
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // 环境光

		{ // 绘制光源
			glm::mat4 model;
			model = glm::translate(model, lightPos);
			model = glm::scale(model, glm::vec3(0.2));
			shader[0].use(); // 显示光源的小白块 lesson_01_color_light
			glBindVertexArray(VAO[0]);
			shader[0].setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));
			shader[0].setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));
			shader[0].setMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));
			shader[0].setVec3("lightColor", lightColor);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// 绘制 box // 聚光软化
		if (true) {
			shader[5].use(); // lesson_04_light_casters_spotlight_soft

			shader[5].setInt("material.diffuse", 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture[0]);

			shader[5].setInt("material.specular", 1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture[1]);

			shader[5].setVec3("light.position", camera->Position);
			shader[5].setVec3("light.direction", camera->Front);
			shader[5].setFloat("light.cutOff", glm::cos(glm::radians(8.5f)));
			shader[5].setFloat("light.outerCutOff", glm::cos(glm::radians(13.5f)));

			glBindVertexArray(VAO[1]);

			shader[5].setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));
			shader[5].setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));
			shader[5].setVec3("viewPos", camera->Position);

			shader[5].setFloat("material.shininess", 32.0f);

			shader[5].setVec3("light.ambient", ambientColor);
			shader[5].setVec3("light.diffuse", diffuseColor);
			shader[5].setVec3("light.specular", glm::vec3(1.0f));
			shader[5].setBool("useSpotLight", useSpotLight);

			glm::vec3 cubePositions[] = {
				glm::vec3(0.0f,  0.0f,  0.0f),
				glm::vec3(2.0f,  5.0f, -15.0f),
				glm::vec3(-1.5f, -2.2f, -2.5f),
				glm::vec3(-3.8f, -2.0f, -12.3f),
				glm::vec3(2.4f, -0.4f, -3.5f),
				glm::vec3(-1.7f,  3.0f, -7.5f),
				glm::vec3(1.3f, -2.0f, -2.5f),
				glm::vec3(1.5f,  2.0f, -2.5f),
				glm::vec3(1.5f,  0.2f, -1.5f),
				glm::vec3(-1.3f,  1.0f, -1.5f)
			};

			for (int i = 0; i < sizeof(cubePositions) / sizeof(glm::vec3); i++) {
				//if (0 != i) {
				//	continue;
				//}
				glm::mat4 model;
				model = glm::translate(model, cubePositions[i]);
				GLfloat angle = 20.0f * i;
				GLfloat step = 0.0f;
				//if (0 == i % 3) {
				//	step = (GLfloat)glfwGetTime() * 20.0f;
				//}
				model = glm::rotate(model, step + angle, glm::vec3(1.0f, 0.3f, 0.5f));
				shader[5].setMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}

		// 绘制 box // 聚光
		if (false) {
			shader[4].use();

			shader[4].setInt("material.diffuse", 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture[0]);

			shader[4].setInt("material.specular", 1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture[1]);

			shader[4].setVec3("light.position", camera->Position);
			shader[4].setVec3("light.direction", camera->Front);
			shader[4].setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));

			glBindVertexArray(VAO[1]);

			shader[4].setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));
			shader[4].setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));
			shader[4].setVec3("viewPos", camera->Position);

			shader[4].setFloat("material.shininess", 32.0f);

			shader[4].setVec3("light.ambient", ambientColor);
			shader[4].setVec3("light.diffuse", diffuseColor);
			shader[4].setVec3("light.specular", glm::vec3(1.0f));

			glm::vec3 cubePositions[] = {
				glm::vec3(0.0f,  0.0f,  0.0f),
				glm::vec3(2.0f,  5.0f, -15.0f),
				glm::vec3(-1.5f, -2.2f, -2.5f),
				glm::vec3(-3.8f, -2.0f, -12.3f),
				glm::vec3(2.4f, -0.4f, -3.5f),
				glm::vec3(-1.7f,  3.0f, -7.5f),
				glm::vec3(1.3f, -2.0f, -2.5f),
				glm::vec3(1.5f,  2.0f, -2.5f),
				glm::vec3(1.5f,  0.2f, -1.5f),
				glm::vec3(-1.3f,  1.0f, -1.5f)
			};

			for (int i = 0; i < sizeof(cubePositions) / sizeof(glm::vec3); i++) {
				//if (0 != i) {
				//	continue;
				//}
				glm::mat4 model;
				model = glm::translate(model, cubePositions[i]);
				GLfloat angle = 20.0f * i;
				GLfloat step = 0.0f;
				//if (0 == i % 3) {
				//	step = (GLfloat)glfwGetTime() * 20.0f;
				//}
				model = glm::rotate(model, step + angle, glm::vec3(1.0f, 0.3f, 0.5f));
				shader[4].setMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}

		// 绘制 box // 点光源
		if (false) {
			shader[3].use();

			shader[3].setInt("material.diffuse", 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture[0]);

			shader[3].setInt("material.specular", 1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture[1]);

			shader[3].setVec3("light.position", lightPos);

			glBindVertexArray(VAO[1]);

			shader[3].setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));
			shader[3].setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));
			shader[3].setVec3("viewPos", camera->Position);

			shader[3].setFloat("material.shininess", 32.0f);

			shader[3].setVec3("light.ambient", ambientColor);
			shader[3].setVec3("light.diffuse", diffuseColor);
			shader[3].setVec3("light.specular", glm::vec3(1.0f));

			shader[3].setFloat("light.constant", 1.0f);
			shader[3].setFloat("light.linear", 0.09f);
			shader[3].setFloat("light.quadratic", 0.032f);

			glm::vec3 cubePositions[] = {
				glm::vec3(0.0f,  0.0f,  0.0f),
				glm::vec3(2.0f,  5.0f, -15.0f),
				glm::vec3(-1.5f, -2.2f, -2.5f),
				glm::vec3(-3.8f, -2.0f, -12.3f),
				glm::vec3(2.4f, -0.4f, -3.5f),
				glm::vec3(-1.7f,  3.0f, -7.5f),
				glm::vec3(1.3f, -2.0f, -2.5f),
				glm::vec3(1.5f,  2.0f, -2.5f),
				glm::vec3(1.5f,  0.2f, -1.5f),
				glm::vec3(-1.3f,  1.0f, -1.5f)
			};

			for (int i = 0; i < sizeof(cubePositions) / sizeof(glm::vec3); i++) {
				//if (0 != i) {
				//	continue;
				//}
				glm::mat4 model;
				model = glm::translate(model, cubePositions[i]);
				GLfloat angle = 20.0f * i;
				GLfloat step = 0.0f;
				//if (0 == i % 3) {
				//	step = (GLfloat)glfwGetTime() * 20.0f;
				//}
				model = glm::rotate(model, step + angle, glm::vec3(1.0f, 0.3f, 0.5f));
				shader[3].setMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}

		// 绘制 box // 平行光
		if (false) {
			shader[2].use();

			shader[2].setInt("material.diffuse", 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture[0]);

			shader[2].setInt("material.specular", 1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture[1]);

			shader[2].setVec3("light.direction", -0.2f, -1.0f, -0.3f);

			glBindVertexArray(VAO[1]);

			shader[2].setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));
			shader[2].setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));
			shader[2].setVec3("viewPos", camera->Position);

			shader[2].setFloat("material.shininess", 32.0f);

			shader[2].setVec3("light.ambient", ambientColor);
			shader[2].setVec3("light.diffuse", diffuseColor);
			shader[2].setVec3("light.specular", glm::vec3(1.0f));

			glm::vec3 cubePositions[] = {
				glm::vec3(0.0f,  0.0f,  0.0f),
				glm::vec3(2.0f,  5.0f, -15.0f),
				glm::vec3(-1.5f, -2.2f, -2.5f),
				glm::vec3(-3.8f, -2.0f, -12.3f),
				glm::vec3(2.4f, -0.4f, -3.5f),
				glm::vec3(-1.7f,  3.0f, -7.5f),
				glm::vec3(1.3f, -2.0f, -2.5f),
				glm::vec3(1.5f,  2.0f, -2.5f),
				glm::vec3(1.5f,  0.2f, -1.5f),
				glm::vec3(-1.3f,  1.0f, -1.5f)
			};

			for (int i = 0; i < sizeof(cubePositions) / sizeof(glm::vec3); i++) {
				//if (0 != i) {
				//	continue;
				//}
				glm::mat4 model;
				model = glm::translate(model, cubePositions[i]);
				GLfloat angle = 20.0f * i;
				GLfloat step = 0.0f;
				//if (0 == i % 3) {
				//	step = (GLfloat)glfwGetTime() * 20.0f;
				//}
				model = glm::rotate(model, step + angle, glm::vec3(1.0f, 0.3f, 0.5f));
				shader[2].setMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}

		// 绘制 box // example
		if (false) {
			shader[1].use();

			shader[1].setInt("material.diffuse", 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture[0]);

			shader[1].setInt("material.specular", 1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture[1]);

			glBindVertexArray(VAO[1]);

			shader[1].setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));
			shader[1].setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));
			shader[1].setVec3("light.position", lightPos);
			shader[1].setVec3("viewPos", camera->Position);

			shader[1].setFloat("material.shininess", 32.0f);

			shader[1].setVec3("light.ambient", ambientColor);
			shader[1].setVec3("light.diffuse", diffuseColor);
			shader[1].setVec3("light.specular", glm::vec3(1.0f));

			glm::vec3 cubePositions[] = {
				glm::vec3(0.0f,  0.0f,  0.0f),
				glm::vec3(2.0f,  5.0f, -15.0f),
				glm::vec3(-1.5f, -2.2f, -2.5f),
				glm::vec3(-3.8f, -2.0f, -12.3f),
				glm::vec3(2.4f, -0.4f, -3.5f),
				glm::vec3(-1.7f,  3.0f, -7.5f),
				glm::vec3(1.3f, -2.0f, -2.5f),
				glm::vec3(1.5f,  2.0f, -2.5f),
				glm::vec3(1.5f,  0.2f, -1.5f),
				glm::vec3(-1.3f,  1.0f, -1.5f)
			};

			for (int i = 0; i < sizeof(cubePositions) / sizeof(glm::vec3); i++) {
				//if (0 != i) {
				//	continue;
				//}
				glm::mat4 model;
				model = glm::translate(model, cubePositions[i]);
				GLfloat angle = 20.0f * i;
				GLfloat step = 0.0f;
				//if (0 == i % 3) {
				//	step = (GLfloat)glfwGetTime() * 20.0f;
				//}
				model = glm::rotate(model, step + angle, glm::vec3(1.0f, 0.3f, 0.5f));
				shader[1].setMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}
	}

	virtual void OnOverRender() {
		/*	glDeleteVertexArrays(3, VAO);
			glDeleteBuffers(3, VBO);*/
		delete camera;
	}

	// 按键响应回调
	virtual void OnKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
		if (action == GLFW_PRESS)
		{
			if (key == GLFW_KEY_F) {
				useSpotLight = !useSpotLight;
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
			camera->ProcessKeyboard(Camera_Movement::FORWARD, deltaTime, speedUp);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			camera->ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime, speedUp);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			camera->ProcessKeyboard(Camera_Movement::LEFT, deltaTime, speedUp);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			camera->ProcessKeyboard(Camera_Movement::RIGHT, deltaTime, speedUp);
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			camera->ProcessKeyboard(Camera_Movement::UP, deltaTime, speedUp);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
			camera->ProcessKeyboard(Camera_Movement::DOWN, deltaTime, speedUp);
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
	}
};
