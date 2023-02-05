/*
	2D��Ϸ
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
#include "Game.h"

#include <ft2build.h>
#include FT_FREETYPE_H

// ����ͼƬ
unsigned int loadTexture(unsigned int textureID, char const * path, GLint textureWrapS, GLint textureWrapT, bool isSRGB);
// ������պ�
unsigned int loadCubemap(std::vector<std::string> faces);
float lerp(float a, float b, float f);

class DrawSimpleTriangle_37 : public SimpleDrawTestBase
{
public:
	GLuint SCR_WIDTH = 800;
	GLuint SCR_HEIGHT = 600;
	// ƽ��ͷ��Ŀ�߱� // (float)SCR_WIDTH / (float)SCR_HEIGHT
	GLfloat aspect = 1.0f;

	GLfloat lastX = 400.0f;
	GLfloat lastY = 300.0f;
	GLboolean firstMouse = true;

	bool useSpotLight = true;
	int effectType = 0;
	// ÿһ֡�ļ��ʱ��
	float deltaTime = 0.0f;

#pragma region "��ڿ��õ��ı���"
	Game game;
#pragma endregion

	DrawSimpleTriangle_37(GLuint screenWidth, GLuint screenHeight) {
		this->SCR_WIDTH = screenWidth;
		this->SCR_HEIGHT = screenHeight;
		aspect = (GLfloat)screenWidth / (GLfloat)screenHeight;
		game = Game(screenWidth, screenHeight);
	}

	virtual void OnInitRender(GLFWwindow* window) {
		glfwSetWindowTitle(window, "DrawSimpleTriangle_37");

		// ���ص�ͼ��Ĭ�����·�ת
		stbi_set_flip_vertically_on_load(true);

		{ // ��ڿ��õ��Ĵ���
			game.Init();
		}

		// ������Ȳ���
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// MSAA
		//glEnable(GL_MULTISAMPLE);
		//glDisable(GL_MULTISAMPLE);

		//glEnable(GL_CULL_FACE);
		//glDisable(GL_CULL_FACE);

		//glEnable(GL_PROGRAM_POINT_SIZE);
		//glPointSize(10.5f);

		// �߿�ģʽ(Wireframe Mode)
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

		// 2D��Ϸ
		if (true) {
			game.Update(this->deltaTime);
			game.Render();
		}
	}


	virtual void OnOverRender() {

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
		// ����
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			speedUp = true;
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		}
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			//std::cout << "OnProcessInput, exposure:" << exposure << std::endl;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			//std::cout << "OnProcessInput, exposure:" << exposure << std::endl;
		}
		// 2D��Ϸ
		if (true) {
			game.ProcessInput(this->deltaTime);
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
		float yoffset = lastY - ypos; // ע���������෴�ģ���Ϊy�����Ǵӵײ����������������
		lastX = xpos;
		lastY = ypos;
	}

	virtual void OnScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	}

	virtual void OnFrameBufferSizeChanged(GLFWwindow* window, int width, int height) {
		this->SCR_WIDTH = width;
		this->SCR_HEIGHT = height;
		aspect = (GLfloat)width / (GLfloat)height;
	}

	virtual bool IsHideMouse() { return false; }
};
