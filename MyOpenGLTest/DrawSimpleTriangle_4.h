/*
	着色器。测试变量传递
*/
#pragma once
#include <iostream>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include "../src/Shader.h"
#include "SimpleDrawTestBase.h"

class DrawSimpleTriangle_4 : public SimpleDrawTestBase
{
public:
	// Vertex Array Object, VAO
	GLuint VAO[3];
	// Vertex Buffer Object, VBO
	GLuint VBO[3];
	// shader 程序
	GLuint program[5];
	// ourColor 这个 uniform 变量的位置
	GLint vertextColorLocation;
	GLint offsetPosLocation;

	virtual void OnInitRender(GLFWwindow* window) {
		//std::cout << "call PreRender()" << std::endl;
		GLfloat vertices[] = {
			0.0f, 0.0f, 0.0f,
			-0.5f, 0.0f, 0.0f,
			-0.25f, 0.5f, 0.0f
		};
		GLfloat vertices_2[] = {
			0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.25f, 0.5f, 0.0f
		};
		GLfloat vertices_3[] = {
			// 位置              // 颜色
			 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
			-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
			 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
		};

		// 编译着色器
		Shader myShader("../res/Shaders/linkTest.vs", "../res/Shaders/linkTest.frag");
		Shader myShader_2("../res/Shaders/default.vs", "../res/Shaders/uniformTest.frag");
		Shader myShader_3("../res/Shaders/someColor.vs", "../res/Shaders/someColor.frag");
		Shader myShader_4("../res/Shaders/someColor_1.vs", "../res/Shaders/someColor.frag");
		Shader myShader_5("../res/Shaders/someColor_2.vs", "../res/Shaders/someColor_2.frag");
		program[0] = myShader.ID;
		program[1] = myShader_2.ID;
		program[2] = myShader_3.ID;
		program[3] = myShader_4.ID;
		program[4] = myShader_5.ID;
		// 位置值可以随时获取
		vertextColorLocation = glGetUniformLocation(program[1], "outColor");
		//offsetPosLocation = glGetUniformLocation(program[3], "offsetPos");
		offsetPosLocation = glGetUniformLocation(program[3], "offsetX");
		// 生成 VBO
		glGenBuffers(3, VBO);
		// 创建 VAO
		glGenVertexArrays(3, VAO);

		glBindVertexArray(VAO[0]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		glBindVertexArray(VAO[1]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_2), vertices_2, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		glBindVertexArray(VAO[2]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_3), vertices_3, GL_STATIC_DRAW);
		// vertext
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		// color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);

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
		glUseProgram(program[0]);
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(program[1]);
		GLfloat timeValue = (GLfloat)glfwGetTime();
		GLfloat greenValue = (sin(timeValue) / 2) + 0.5f;
		glUniform4f(vertextColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(program[2]);
		glBindVertexArray(VAO[2]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(program[3]);
		GLfloat offsetPosX = sin(timeValue);
		//glUniform3f(offsetPosLocation, offsetPosX, 0.0f, 0.0f);
		glUniform1f(offsetPosLocation, offsetPosX);
		glBindVertexArray(VAO[2]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(program[4]);
		glBindVertexArray(VAO[2]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);
	}

	virtual void OnOverRender() {
		glDeleteVertexArrays(3, VAO);
		glDeleteBuffers(3, VBO);
	}
};
