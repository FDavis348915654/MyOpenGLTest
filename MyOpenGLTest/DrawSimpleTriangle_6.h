/*
	变换
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

class DrawSimpleTriangle_6 : public SimpleDrawTestBase
{
public:
	// Vertex Array Object, VAO
	GLuint VAO[10];
	// Element Buffer Object, EBO
	GLuint EBO[10];
	// Vertex Buffer Object, VBO
	GLuint VBO[10];
	// shader 程序
	GLuint program[10];
	// 纹理
	GLuint texture[10];
	// ourColor 这个 uniform 变量的位置
	GLint vertextColorLocation;
	GLint offsetPosLocation;

	GLfloat mixValue = 0.5f;
	/*glm::mat4 trans;*/

	virtual void OnInitRender(GLFWwindow* window) {
		//std::cout << "call PreRender()" << std::endl;
		GLfloat vertices[] = {
			//     ---- 位置 ----      - 纹理坐标 -
				 0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // 右上
				 0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // 右下
				-0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // 左下
				-0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // 左上
		};
		// 都用这些索引
		GLuint indices[] = { // 注意索引从0开始! 
			0, 1, 3, // 第一个三角形
			1, 2, 3  // 第二个三角形
		};

		// 编译着色器
		Shader myShader("../res/Shaders/test_1_trans.vs", "../res/Shaders/test_1_trans.fs");
		program[0] = myShader.ID;
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

		{ // 设置顶点属性
			// Vertex Array Object
			glBindVertexArray(VAO[0]);
			// 复制顶点数组到一个顶点缓冲中供 OpenGL 使用
			glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			// 复制索引数组到一个索引缓冲中供 OpenGL 使用
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
			// vertex
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			// uv
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);
			glBindVertexArray(0);
		}

		//{ // 向量平移练习。 矩阵写法, 从右往左看, 先缩放, 后旋转, 最后位移。 translate * rotate * scale
		//	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
		//	glm::mat4 trans;
		//	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
		//	vec = trans * vec;
		//	std::cout << vec.x << vec.y << vec.z << std::endl;

		//	trans = glm::rotate(trans, 90.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		//	trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
		//}

		//GLfloat angle = 90.0f;
		//GLfloat scale = 0.5f;
		//glm::mat4 trans;
		////trans = glm::rotate(trans, angle, glm::vec3(0.0f, 0.0f, 1.0f));
		//trans = glm::rotate(trans, angle, glm::vec3(0.0f, 0.0f, 1.0f));
		//trans = glm::scale(trans, glm::vec3(scale, scale, scale));

		// 线框模式(Wireframe Mode)
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	virtual void OnPreRender(float deltaTime) {
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	virtual void OnRender() {
		//std::cout << "call OnRender()" << std::endl;

		// 绘制纹理
		if (true) {
			glUseProgram(program[0]);

			// 测试一
			//GLfloat angle = 90.0f;
			//GLfloat scale = 0.5f;
			//glm::mat4 trans;
			////trans = glm::rotate(trans, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
			//trans = glm::rotate(trans, angle, glm::vec3(0.0f, 0.0f, 1.0f)); // 有些 glm 库是直接传角度的, 不需要包 glm::radians(angle)
			//trans = glm::scale(trans, glm::vec3(scale, scale, scale));

			// 测试二
			//GLfloat angle = 15.0f + (GLfloat)glfwGetTime() * 25;
			//glm::mat4 trans;
			//trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
			//trans = glm::rotate(trans, angle, glm::vec3(0.0f, 0.0f, 1.0f)); // 有些 glm 库是直接传角度的

			// 练习题一, 使用应用在箱子上的最后一个变换，尝试将其改变为先旋转，后位移。看看发生了什么，试着想想为什么会发生这样的事情
			GLfloat angle = 15.0f + (GLfloat)glfwGetTime() * 25;
			glm::mat4 trans;
			trans = glm::rotate(trans, angle, glm::vec3(0.0f, 0.0f, 1.0f));
			//trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
			trans = glm::translate(trans, glm::vec3(0.5f, 0.0f, 0.0f));

			GLuint transformLoc = glGetUniformLocation(program[0], "transform");
			/*
				第一个参数你现在应该很熟悉了，它是uniform的位置值。
				第二个参数告诉OpenGL我们将要发送多少个矩阵，这里是1。
				第三个参数询问我们是否希望对我们的矩阵进行转置(Transpose)，也就是说交换我们矩阵的行和列。OpenGL开发者通常使用一种内部矩阵布局，叫做列主序(Column-major Ordering)布局。GLM的默认布局就是列主序，所以并不需要转置矩阵，我们填GL_FALSE。
				最后一个参数是真正的矩阵数据，但是GLM并不是把它们的矩阵储存为OpenGL所希望接受的那种，因此我们要先用GLM的自带的函数value_ptr来变换这些数据。
			*/
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture[0]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture[1]);
			glUniform1i(glGetUniformLocation(program[0], "texture1"), 0);
			glUniform1i(glGetUniformLocation(program[0], "texture2"), 1);
			glBindVertexArray(VAO[0]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			// 练习题二, 尝试再次调用glDrawElements画出第二个箱子，只使用变换将其摆放在不同的位置。让这个箱子被摆放在窗口的左上角，并且会不断的缩放（而不是旋转）。（sin函数在这里会很有用，不过注意使用sin函数时应用负值会导致物体被翻转）
			GLfloat scale = sin((GLfloat)glfwGetTime() * 1) + 0.25f;
			trans = glm::mat4(1.0f);
			trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
			trans = glm::scale(trans, glm::vec3(scale, scale, scale));
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		// 每一帧绘制结束时取消绑定(也可以不取消)
		glBindVertexArray(0);
	}

	virtual void OnOverRender() {
		/*	glDeleteVertexArrays(3, VAO);
			glDeleteBuffers(3, VBO);*/
	}

	virtual void OnKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
		const GLfloat step = 0.01f;
		if (action == GLFW_PRESS || action == GLFW_REPEAT)
		{
			if (key == GLFW_KEY_UP) {
				mixValue += step;
				if (mixValue > 1.0f) {
					mixValue = 1.0f;
				}
				std::cout << "GLFW_KEY_UP, mixValue: " << mixValue << std::endl;
			}
			else if (key == GLFW_KEY_DOWN) {
				mixValue -= step;
				if (mixValue < 0.0f) {
					mixValue = 0.0f;
				}
				std::cout << "GLFW_KEY_DOWN, mixValue: " << mixValue << std::endl;
			}
		}
	}
};
