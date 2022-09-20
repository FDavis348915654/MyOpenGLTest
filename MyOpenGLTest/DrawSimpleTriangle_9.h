/*
	颜色
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

class DrawSimpleTriangle_9 : public SimpleDrawTestBase
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

	// ourColor 这个 uniform 变量的位置
	GLint vertextColorLocation;
	GLint offsetPosLocation;
	GLuint screenWidth;
	GLuint screenHeight;

	GLfloat fov = 45.0f;
	GLfloat aspect = 1;

	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;

	// 偏航角
	GLfloat yaw = 0.0f;
	// 俯仰角
	GLfloat pitch = 0.0f;
	// 滚转角
	GLfloat roll = 0.0f;

	GLfloat lastX = 400.0f;
	GLfloat lastY = 300.0f;
	GLboolean firstMouse = true;

	DrawSimpleTriangle_9(GLuint screenWidth, GLuint screenHeight) {
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;
		aspect = (GLfloat)screenWidth / (GLfloat)screenHeight;
		cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	}

	virtual void OnInitRender(GLFWwindow* window) {
		//std::cout << "call PreRender()" << std::endl;
		float verticesBox[] = {
			//---- 位置 ----     - 纹理坐标 -
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

		// 编译着色器
		shader[0] = Shader("../res/Shaders/test_1_color_light.vs", "../res/Shaders/test_1_color_light.fs");
		shader[1] = Shader("../res/Shaders/test_1_color.vs", "../res/Shaders/test_1_color.fs");
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

		{ // 绑定一个纹理对象, 为当前绑定的纹理对象设置环绕、过滤方式 // 木箱
			glBindTexture(GL_TEXTURE_2D, texture[0]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// 加载并生成纹理
			int width, height, nrChannels;
			const char* fileName = "../res/Texture/container.jpg";
			unsigned char *data = stbi_load(fileName, &width, &height, &nrChannels, 0);
			if (data) {
				std::cout << "load texture success, fileName: " << fileName << std::endl;
				/*
				函数很长，参数也不少，所以我们一个一个地讲解：
					第一个参数指定了纹理目标(Target)。设置为GL_TEXTURE_2D意味着会生成与当前绑定的纹理对象在同一个目标上的纹理（任何绑定到GL_TEXTURE_1D和GL_TEXTURE_3D的纹理不会受到影响）。
					第二个参数为纹理指定多级渐远纹理的级别，如果你希望单独手动设置每个多级渐远纹理的级别的话。这里我们填0，也就是基本级别。
					第三个参数告诉OpenGL我们希望把纹理储存为何种格式。我们的图像只有RGB值，因此我们也把纹理储存为RGB值。
					第四个和第五个参数设置最终的纹理的宽度和高度。我们之前加载图像的时候储存了它们，所以我们使用对应的变量。
					下个参数应该总是被设为0（历史遗留的问题）。
					第七第八个参数定义了源图的格式和数据类型。我们使用RGB值加载这个图像，并把它们储存为char(byte)数组，我们将会传入对应值。
					最后一个参数是真正的图像数据。
					当调用glTexImage2D时，当前绑定的纹理对象就会被附加上纹理图像。然而，目前只有基本级别(Base-level)的纹理图像被加载了，如果要使用多级渐远纹理，我们必须手动设置所有不同的图像（不断递增第二个参数）。或者，直接在生成纹理之后调用glGenerateMipmap。这会为当前绑定的纹理自动生成所有需要的多级渐远纹理。
				*/
				// 生成纹理
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else {
				std::cout << "error, Failed to load texture, fileName: " << fileName << std::endl;
			}
			stbi_image_free(data);
		}

		{ // 绑定一个纹理对象, 为当前绑定的纹理对象设置环绕、过滤方式 // 笑脸
			glBindTexture(GL_TEXTURE_2D, texture[1]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// 加载并生成纹理
			int width, height, nrChannels;
			const char* fileName = "../res/Texture/awesomeface.png";
			unsigned char *data = stbi_load(fileName, &width, &height, &nrChannels, 0);
			if (data) {
				std::cout << "load texture success, fileName: " << fileName << std::endl;
				// 生成纹理
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else {
				std::cout << "error, Failed to load texture, fileName: " << fileName << std::endl;
			}
			stbi_image_free(data);
		}

		{ // 设置顶点属性 // 灯
			// Vertex Array Object
			glBindVertexArray(VAO[0]);
			// 复制顶点数组到一个顶点缓冲中供 OpenGL 使用
			glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(verticesBox), verticesBox, GL_STATIC_DRAW);
			// vertex
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			glBindVertexArray(0);
		}

		{ // 设置顶点属性 // Box
			// Vertex Array Object
			glBindVertexArray(VAO[1]);
			// 复制顶点数组到一个顶点缓冲中供 OpenGL 使用
			glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(verticesBox), verticesBox, GL_STATIC_DRAW);
			// vertex
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			// uv
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);
			glBindVertexArray(0);
		}

		// 开启深度测试
		glEnable(GL_DEPTH_TEST);
	}

	virtual void OnPreRender(float deltaTime) {
		// Clear the colorbuffer
		if (false) {
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		// z 缓冲
		if (true) {
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
	}

	virtual void OnRender() {
		//std::cout << "call OnRender()" << std::endl;
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glm::mat4 projection = glm::perspective(fov, aspect, 0.1f, 100.0f);

		// 绘制光源
		if (true) {
			glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
			glm::mat4 model;
			model = glm::translate(model, lightPos);
			model = glm::scale(model, glm::vec3(0.2));
			shader[0].use();
			glBindVertexArray(VAO[0]);
			shader[0].setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));
			shader[0].setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));
			shader[0].setMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// 绘制纹理 box // 相机自由移动
		if (true) {
			shader[1].use();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture[0]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture[1]);
			shader[1].setInt("texture1", 0);
			shader[1].setInt("texture2", 1);

			glBindVertexArray(VAO[1]);

			shader[1].setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));
			shader[1].setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));
			shader[1].setVec3("objectColor", 1.0f, 0.5f, 0.31f);
			shader[1].setVec3("lightColor", 1.0f, 1.0f, 1.0f);

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
				if (0 != i) {
					continue;
				}
				glm::mat4 model;
				model = glm::translate(model, cubePositions[i]);
				GLfloat angle = 20.0f * i;
				GLfloat step = 0.0f;
				if (0 == i % 3) {
					step = (GLfloat)glfwGetTime() * 50.0f;
				}
				model = glm::rotate(model, step + angle, glm::vec3(1.0f, 0.3f, 0.5f));
				shader[1].setMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}
	}

	virtual void OnOverRender() {
		/*	glDeleteVertexArrays(3, VAO);
			glDeleteBuffers(3, VBO);*/
	}

	virtual void OnProcessInput(GLFWwindow* window) {
		GLfloat cameraSpeed = 0.05f;
		// 加速
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			cameraSpeed *= 2.0f;
		}

		// 自由相机
		if (true) {
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
				cameraPos += cameraSpeed * cameraFront;
			}
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
				cameraPos -= cameraSpeed * cameraFront;
			}
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
				cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
			}
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
				cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
			}
		}

		// fps 相机 // 练习题一, 看看你是否能够修改摄像机类，使得其能够变成一个真正的FPS摄像机（也就是说不能够随意飞行）；你只能够呆在xz平面上
		if (false) {
			glm::vec3 xzFront = cameraFront;
			xzFront.y = 0;
			xzFront = glm::normalize(xzFront);

			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
				cameraPos += cameraSpeed * xzFront;
			}
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
				cameraPos -= cameraSpeed * xzFront;
			}
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
				cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
			}
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
				cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
			}
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			cameraPos += cameraSpeed * cameraUp;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
			cameraPos -= cameraSpeed * cameraUp;
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

		float sensitivity = 0.05f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.9f) {
			pitch = 89.9f;
		}
		if (pitch < -89.9f) {
			pitch = -89.9f;
		}

		glm::vec3 front;
		front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw - 90.0f));
		front.y = sin(glm::radians(pitch));
		front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw - 90.0f));
		//printf("front(%f, %f, %f)\n", front.x, front.y, front.z);
		cameraFront = glm::normalize(front);
	}

	virtual void OnScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
		if (fov >= 1.0f && fov <= 45.0f) {
			fov -= yoffset;
		}
		if (fov <= 1.0f) {
			fov = 1.0f;
		}
		if (fov >= 45.0f) {
			fov = 45.0f;
		}
	}
};
