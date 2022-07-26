/*
	你好，三角形。练习
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

class DrawSimpleTriangle_3 : public SimpleDrawTestBase
{
public:
	// Vertex Array Object, VAO
	GLuint VAO[2];
	// 生成 VBO
	GLuint VBO[2];
	// shader 程序
	GLuint program[2];

	virtual void OnInitRender(GLFWwindow* window) {
		//std::cout << "call PreRender()" << std::endl;
		GLfloat vertices[] = {
			0.0f, 0.0f, 0.0f,
			-0.5f, 0.0f, 0.0f,
			-0.25f, 0.5f, 0.0f
		};
		GLfloat vertices2[] = {
			0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f,
			0.25f, 0.5f, 0.0f
		};
		// 编译着色器
		Shader myShader("../res/Shaders/default.vs", "../res/Shaders/default.frag");
		Shader myShader2("../res/Shaders/default.vs", "../res/Shaders/default_2.frag");
		program[0] = myShader.program;
		program[1] = myShader2.program;
		// 生成 VBO
		glGenBuffers(2, VBO);
		// 创建 VAO
		glGenVertexArrays(2, VAO);

		glBindVertexArray(VAO[0]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		glBindVertexArray(VAO[1]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
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
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
	}

	virtual void OnOverRender() {
		glDeleteVertexArrays(2, VAO);
		glDeleteBuffers(2, VBO);
	}
};
