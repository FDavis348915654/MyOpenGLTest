/*
	֡����
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

class DrawSimpleTriangle_20 : public SimpleDrawTestBase
{
public:
	// Vertex Array Object, VAO
	GLuint VAO[10];
	// Element Buffer Object, EBO
	GLuint EBO[10];
	// Vertex Buffer Object, VBO
	GLuint VBO[10];
	// �������
	GLuint texture[10];
	// Shader ������
	Shader shader[10];
	// camera
	Camera* camera;
	// Framebuffer Object // ֡�������
	unsigned int fbo;
	// Renderbuffer Object // ��Ⱦ�������
	unsigned int rbo;
	// ������
	unsigned int texColorBuffer;

	GLuint screenWidth;
	GLuint screenHeight;

	// ƽ��ͷ��Ŀ�߱�
	GLfloat aspect = 1;

	GLfloat lastX = 400.0f;
	GLfloat lastY = 300.0f;
	GLboolean firstMouse = true;

	bool useSpotLight = true;

	float deltaTime = 0.0f;

	int effectType = 0;

	DrawSimpleTriangle_20(GLuint screenWidth, GLuint screenHeight) {
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;
		aspect = (GLfloat)screenWidth / (GLfloat)screenHeight;
	}

	virtual void OnInitRender(GLFWwindow* window) {
		glfwSetWindowTitle(window, "DrawSimpleTriangle_20");
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
		float transparentVertices[] = {
			// positions         // texture Coords
			0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
			0.0f, -0.5f,  0.0f,  0.0f,  0.0f,
			1.0f, -0.5f,  0.0f,  1.0f,  0.0f,

			0.0f,  0.5f,  0.0f,  0.0f,  1.0f,
			1.0f, -0.5f,  0.0f,  1.0f,  0.0f,
			1.0f,  0.5f,  0.0f,  1.0f,  1.0f
		};
		float quadVertices[] = {
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};

		// ������ɫ��
		shader[0] = Shader("../res/Shaders/lesson_06_depth_test.vs", "../res/Shaders/lesson_06_depth_test.frag");
		shader[1] = Shader("../res/Shaders/lesson_08_blend_test.vs", "../res/Shaders/lesson_08_blend_test.frag"); // �ݴ�
		shader[2] = Shader("../res/Shaders/lesson_08_blend_test.vs", "../res/Shaders/lesson_08_blend_test_1.frag"); // ������
		shader[3] = Shader("../res/Shaders/lesson_09_framebuffers.vs", "../res/Shaders/lesson_09_framebuffers.frag"); // ��Ļ
		// ���� VBO
		glGenBuffers(10, VBO);
		// ���� EBO
		glGenBuffers(10, EBO);
		// ���� VAO
		glGenVertexArrays(10, VAO);
		// �����������
		glGenTextures(10, texture);
		// ���ص�ͼ��Ĭ�����·�ת
		stbi_set_flip_vertically_on_load(true);

		{ // ���ö������� // ����
			// Vertex Array Object
			glBindVertexArray(VAO[0]);
			// ���ƶ������鵽һ�����㻺���й� OpenGL ʹ��
			glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			glBindVertexArray(0);
		}

		{ // ���ö������� // ƽ��
			// Vertex Array Object
			glBindVertexArray(VAO[1]);
			// ���ƶ������鵽һ�����㻺���й� OpenGL ʹ��
			glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			glBindVertexArray(0);
		}

		{ // ���ö������� // �ݴ�
			// Vertex Array Object
			glBindVertexArray(VAO[2]);
			// ���ƶ������鵽һ�����㻺���й� OpenGL ʹ��
			glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			glBindVertexArray(0);
		}

		{ // ��Ļ
			// Vertex Array Object
			glBindVertexArray(VAO[3]);
			// ���ƶ������鵽һ�����㻺���й� OpenGL ʹ��
			glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
			glBindVertexArray(0);
		}

		// ��һ���������, Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ // ľ��
		loadTexture(texture[0], "../res/Texture/container2.png", GL_REPEAT, GL_REPEAT);
		// ��һ���������, Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ // �߹�
		loadTexture(texture[1], "../res/Texture/container2_specular.png", GL_REPEAT, GL_REPEAT);
		// ��һ���������, Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ // �ݴ�
		loadTexture(texture[2], "../res/Texture/grass.png", GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
		// ��һ���������, Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ // ������
		loadTexture(texture[3], "../res/Texture/blending_transparent_window.png", GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

		// ������Ȳ���
		glEnable(GL_DEPTH_TEST);
		// ������Ȼ���д��
		//glDepthMask(GL_FALSE);
		/*
			����	����
			GL_ALWAYS	��Զͨ����Ȳ���
			GL_NEVER	��Զ��ͨ����Ȳ���
			GL_LESS		��Ƭ�����ֵС�ڻ�������ֵʱͨ������
			GL_EQUAL	��Ƭ�����ֵ���ڻ����������ֵʱͨ������
			GL_LEQUAL	��Ƭ�����ֵС�ڵ��ڻ����������ֵʱͨ������
			GL_GREATER	��Ƭ�����ֵ���ڻ����������ֵʱͨ������
			GL_NOTEQUAL	��Ƭ�����ֵ�����ڻ����������ֵʱͨ������
			GL_GEQUAL	��Ƭ�����ֵ���ڵ��ڻ����������ֵʱͨ������
		*/
		glDepthFunc(GL_LESS);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// ����������˳ʱ�뻹����ʱ��
		// Ĭ��ֵ��GL_CCW�������������ʱ��Ļ���˳����һ��ѡ����GL_CW��������Ȼ���������˳ʱ��˳��
		//glFrontFace(GL_CCW);

		glEnable(GL_CULL_FACE);
		/*
			glCullFace�������������õ�ѡ�

			GL_BACK��ֻ�޳������档
			GL_FRONT��ֻ�޳������档
			GL_FRONT_AND_BACK���޳�������ͱ����档
			glCullFace�ĳ�ʼֵ��GL_BACK��
		*/
		//glCullFace(GL_BACK);

		// ����һ��֡������� // Framebuffer Object
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		{ // Framebuffer Object // ������
			glGenTextures(1, &texColorBuffer);
			glBindTexture(GL_TEXTURE_2D, texColorBuffer);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glBindTexture(GL_TEXTURE_2D, 0);
			/*
				glFrameBufferTexture2D�����µĲ�����
					target��֡�����Ŀ�꣨���ơ���ȡ�������߽��У�
					attachment��������Ҫ���ӵĸ������͡���ǰ�������ڸ���һ����ɫ������ע������0��ζ�����ǿ��Ը��Ӷ����ɫ���������ǽ���֮��Ľ̳����ᵽ��
					textarget����ϣ�����ӵ���������
					texture��Ҫ���ӵ�������
					level���༶��Զ����ļ������ǽ�������Ϊ0��
			*/
			// �������ӵ���ǰ�󶨵�֡�������
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

			// ��һ����Ⱥ�ģ�建�帽��Ϊһ������֡����
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, screenWidth, screenHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
			//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texColorBuffer, 0);
		}

		{ // Renderbuffer Object // ��Ⱦ�������
			glGenRenderbuffers(1, &rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			// ����һ����Ⱥ�ģ����Ⱦ�������
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			// ����Ⱦ������󸽼ӵ�֡�������Ⱥ�ģ�帽����
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
		}

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

		// ��һ����׶�(Pass)
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // �������ڲ�ʹ��ģ�建��
		glEnable(GL_DEPTH_TEST);

		glm::mat4 view = camera->GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera->Zoom, aspect, 0.1f, 100.0f);

		{ // floor
			glDisable(GL_CULL_FACE);
			shader[0].use();
			shader[0].setMat4("view", view);
			shader[0].setMat4("projection", projection);
			glBindVertexArray(VAO[1]);
			glBindTexture(GL_TEXTURE_2D, texture[1]);
			shader[0].setInt("texture_diffuse1", 0);
			glm::mat4 model = glm::mat4(1.0f);
			shader[0].setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glEnable(GL_CULL_FACE);
		}

		{ // cubes
			shader[0].use();
			shader[0].setMat4("view", view);
			shader[0].setMat4("projection", projection);
			glm::mat4 model = glm::mat4(1.0f);
			glBindVertexArray(VAO[0]);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture[0]);
			shader[0].setInt("texture_diffuse1", 0);
			model = glm::translate(model, glm::vec3(-1.0f, 0.001f, -1.0f));
			shader[0].setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(2.0f, 0.001f, 0.0f));
			shader[0].setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		if (true) { // grass
			glDisable(GL_CULL_FACE);
			std::vector<glm::vec3> vegetation;
			vegetation.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
			vegetation.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
			vegetation.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
			vegetation.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
			vegetation.push_back(glm::vec3(0.5f, 0.0f, -0.6f));
			shader[1].use();
			shader[1].setMat4("view", view);
			shader[1].setMat4("projection", projection);
			glBindVertexArray(VAO[2]);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture[2]);
			shader[1].setInt("texture_diffuse1", 0);
			glm::mat4 model = glm::mat4(1.0f);
			for (unsigned int i = 0; i < vegetation.size(); i++) {
				model = glm::mat4(1.0f);
				model = glm::translate(model, vegetation[i]);
				shader[1].setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			glEnable(GL_CULL_FACE);
		}

		if (false) { // window
			glDisable(GL_CULL_FACE);
			std::vector<glm::vec3> windows;
			windows.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
			windows.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
			windows.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
			windows.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
			windows.push_back(glm::vec3(0.5f, 0.0f, -0.6f));
			shader[2].use();
			shader[2].setMat4("view", view);
			shader[2].setMat4("projection", projection);
			glBindVertexArray(VAO[2]);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture[3]);
			shader[2].setInt("texture_diffuse1", 0);
			glm::mat4 model = glm::mat4(1.0f);
			std::map<float, glm::vec3> sorted;
			// ��͸�������谴����������Ⱦ, ����ȾԶ����
			for (unsigned int i = 0; i < windows.size(); i++) {
				float distance = glm::length(camera->Position - windows[i]);
				sorted[distance] = windows[i];
			}
			for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it) {
				model = glm::mat4(1.0f);
				model = glm::translate(model, it->second);
				shader[2].setMat4("model", model);
				glDrawArrays(GL_TRIANGLES, 0, 6);
			}
			// ���������, ��ô��������Ĳ����ͻḲ��Զ������Ĳ���
			//for (unsigned int i = 0; i < windows.size(); i++) {
			//	model = glm::mat4(1.0f);
			//	model = glm::translate(model, windows[i]);
			//	shader[1].setMat4("model", model);
			//	glDrawArrays(GL_TRIANGLES, 0, 6);
			//}
			glEnable(GL_CULL_FACE);
		}

		// �ڶ�����׶�
		glBindFramebuffer(GL_FRAMEBUFFER, 0); // ����Ĭ��
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader[3].use();
		shader[3].setInt("effectType", effectType);
		glBindVertexArray(VAO[3]);
		glDisable(GL_DEPTH_TEST);
		glBindTexture(GL_TEXTURE_2D, texColorBuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);
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
