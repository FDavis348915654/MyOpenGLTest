/*
	模型
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
#include "Model.h"
#include "stb_image.h"
#include "SimpleDrawTestBase.h"
#include "Camera.h"

class DrawSimpleTriangle_15 : public SimpleDrawTestBase
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

	bool useSpotLight = true;

	float deltaTime = 0.0f;

	Model ourModel;

	DrawSimpleTriangle_15(GLuint screenWidth, GLuint screenHeight) {
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;
		aspect = (GLfloat)screenWidth / (GLfloat)screenHeight;
	}

	virtual void OnInitRender(GLFWwindow* window) {
		glfwSetWindowTitle(window, "DrawSimpleTriangle_15");
		camera = new Camera((glm::vec3(0.0f, 0.0f, 3.0f)));
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
		shader[0] = Shader("../res/Shaders/lesson_01_color_light.vs", "../res/Shaders/lesson_01_color_light.frag"); // 用于显示光源的小白块
		shader[1] = Shader("../res/Shaders/lesson_05_model.vs", "../res/Shaders/lesson_05_model.frag");
		shader[2] = Shader("../res/Shaders/lesson_05_model.vs", "../res/Shaders/lesson_05_model_light.frag");
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

		ourModel = Model("../res/nanosuit/nanosuit.obj");

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
		char str[256];
		//std::cout << "call OnRender()" << std::endl;
		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera->Zoom, aspect, 0.1f, 100.0f);
		// 环境光方向
		glm::vec3 lightDir(-0.2f, -1.0f, -0.3f);
		// 光源位置
		glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
		// 光源位置
		glm::vec3 pointLightPositions[] = {
			glm::vec3(0.7f,  0.2f,  2.0f),
			glm::vec3(2.3f, -3.3f, -4.0f),
			glm::vec3(-4.0f,  2.0f, -12.0f),
			glm::vec3(0.0f,  0.0f, -3.0f)
		};
		// 光源颜色(环境光)
		glm::vec3 lightColor(1.0f, 1.0f, 1.0f); // 光源颜色
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // 漫反射
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // 环境光
		// 点光源衰减系数
		float lightConstant = 1.0f;
		float lightLinear = 0.09f;
		float lightQuadratic = 0.032f;

		// 绘制光源
		for (int i = 0; i < sizeof(pointLightPositions) / sizeof(glm::vec3); i++) {
			shader[0].use();
			glm::mat4 model;
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2));
			glBindVertexArray(VAO[0]);
			shader[0].setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));
			shader[0].setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));
			shader[0].setMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));
			shader[0].setVec3("lightColor", lightColor);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// 绘制纳米生化装
		if (true) {
			shader[1].use();
			shader[1].setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));
			shader[1].setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));
			glm::mat4 model;
			model = glm::translate(model, glm::vec3(1.0f, -1.0f, -1.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			shader[1].setMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));

			ourModel.Draw(shader[1]);
		}

		// 绘制纳米生化装
		if (true) {
			shader[2].use();
			shader[2].setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));
			shader[2].setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));
			glm::mat4 model;
			model = glm::translate(model, glm::vec3(0.0f, -1.0f, -1.0f));
			model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
			shader[2].setMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));
			shader[2].setFloat("shininess", 32.0f);

			shader[2].setVec3("viewPos", camera->Position);
			shader[2].setVec3("dirLight.direction", lightDir);
			shader[2].setVec3("dirLight.ambient", ambientColor);
			shader[2].setVec3("dirLight.diffuse", diffuseColor);
			shader[2].setVec3("dirLight.specular", lightColor);

			for (int i = 0; i < sizeof(pointLightPositions) / sizeof(glm::vec3); i++) {
				sprintf_s(str, "pointLights[%d].%s", i, "position");
				shader[2].setVec3(str, pointLightPositions[i]);
				sprintf_s(str, "pointLights[%d].%s", i, "constant");
				shader[2].setFloat(str, lightConstant);
				sprintf_s(str, "pointLights[%d].%s", i, "linear");
				shader[2].setFloat(str, lightLinear);
				sprintf_s(str, "pointLights[%d].%s", i, "quadratic");
				shader[2].setFloat(str, lightQuadratic);
				sprintf_s(str, "pointLights[%d].%s", i, "ambient");
				shader[2].setVec3(str, ambientColor);
				sprintf_s(str, "pointLights[%d].%s", i, "diffuse");
				shader[2].setVec3(str, diffuseColor);
				sprintf_s(str, "pointLights[%d].%s", i, "specular");
				shader[2].setVec3(str, lightColor);
			}

			shader[2].setVec3("spotLight.position", camera->Position);
			shader[2].setVec3("spotLight.direction", camera->Front);
			shader[2].setFloat("spotLight.cutOff", glm::cos(glm::radians(8.5f)));
			shader[2].setFloat("spotLight.outerCutOff", glm::cos(glm::radians(13.5f)));
			shader[2].setVec3("spotLight.ambient", ambientColor);
			shader[2].setVec3("spotLight.diffuse", diffuseColor);
			shader[2].setVec3("spotLight.specular", lightColor);
			shader[2].setBool("useSpotLight", useSpotLight);

			ourModel.Draw(shader[2]);
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
