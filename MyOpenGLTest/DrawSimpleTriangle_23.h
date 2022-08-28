/*
	������ɫ��
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

// ����ͼƬ
unsigned int loadTexture(unsigned int textureID, char const * path, GLint textureWrapS, GLint textureWrapT);
// ������պ�
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
	// ��������
	GLuint texture[10];
	// Shader ������
	Shader shader[10];
	// camera
	Camera* camera;
	// Framebuffer Object // ֡�������
	unsigned int fbo;
	// Renderbuffer Object // ��Ⱦ�������/��Ⱦ����
	unsigned int rbo;
	// ��������
	unsigned int texColorBuffer;
	// ��������ͼ
	unsigned int cubeTexture;
	// uniform buffer object
	unsigned int uboMatrices;

	Model ourModel;

	GLuint screenWidth;
	GLuint screenHeight;

	// ƽ��ͷ��Ŀ��߱�
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
		camera = new Camera((glm::vec3(0.0f, 0.0f, 3.0f)));
		float points[] = {
			-0.5f,  0.5f, // ����
			 0.5f,  0.5f, // ����
			 0.5f, -0.5f, // ����
			-0.5f, -0.5f  // ����
		};

		// ������ɫ��
		shader[0] = Shader("../res/Shaders/lesson_12_geometry.vs", "../res/Shaders/lesson_12_geometry.gs", "../res/Shaders/lesson_12_geometry.frag"); // ����
		//shader[0] = Shader("../res/Shaders/lesson_12_geometry.vs", "../res/Shaders/lesson_12_geometry_test.gs", "../res/Shaders/lesson_12_geometry.frag"); // �ѵ�����
		shader[1] = Shader("../res/Shaders/lesson_12_geometry.vs", "../res/Shaders/lesson_12_geometry_build_house.gs", "../res/Shaders/lesson_12_geometry.frag"); // ����
		// ���� VBO
		glGenBuffers(10, VBO);
		// ���� EBO
		glGenBuffers(10, EBO);
		// ���� VAO
		glGenVertexArrays(10, VAO);
		// ������������
		glGenTextures(10, texture);
		// ���ص�ͼ��Ĭ�����·�ת
		stbi_set_flip_vertically_on_load(true);

		{ // ���ö������� // ƽ��
			// Vertex Array Object
			glBindVertexArray(VAO[0]);
			// ���ƶ������鵽һ�����㻺���й� OpenGL ʹ��
			glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			glBindVertexArray(0);
		}

		// ��һ����������, Ϊ��ǰ�󶨵������������û��ơ����˷�ʽ // ľ��
		loadTexture(texture[0], "../res/Texture/container2.png", GL_REPEAT, GL_REPEAT);
		// ��һ����������, Ϊ��ǰ�󶨵������������û��ơ����˷�ʽ // �߹�
		loadTexture(texture[1], "../res/Texture/container2_specular.png", GL_REPEAT, GL_REPEAT);
		// ��һ����������, Ϊ��ǰ�󶨵������������û��ơ����˷�ʽ // �ݴ�
		loadTexture(texture[2], "../res/Texture/grass.png", GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
		// ��һ����������, Ϊ��ǰ�󶨵������������û��ơ����˷�ʽ // ������
		loadTexture(texture[3], "../res/Texture/blending_transparent_window.png", GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

		// ������Ȳ���
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//glEnable(GL_CULL_FACE);

		//glEnable(GL_PROGRAM_POINT_SIZE);
		//glPointSize(10.5f);

		// �߿�ģʽ(Wireframe Mode)
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

		// 4 ����
		if (false) {
			shader[0].use();
			glBindVertexArray(VAO[0]);
			glDrawArrays(GL_POINTS, 0, 4);
		}

		// 4 ������
		if (true) {
			shader[1].use();
			glBindVertexArray(VAO[0]);
			glDrawArrays(GL_POINTS, 0, 4);
		}

		if (false) { // cubes // ��ͨ������
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
		// ����
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
		float yoffset = lastY - ypos; // ע���������෴�ģ���Ϊy�����Ǵӵײ����������������
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