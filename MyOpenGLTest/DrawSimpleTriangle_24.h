/*
	ʵ����
*/
#pragma once
#include <string>
#include <iostream>
#include <sstream>
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
unsigned int loadTexture(unsigned int textureID, char const * path, GLint textureWrapS, GLint textureWrapT, bool isSRGB);
// ������պ�
unsigned int loadCubemap(std::vector<std::string> faces);

class DrawSimpleTriangle_24 : public SimpleDrawTestBase
{
public:
	// Vertex Array Object, VAO
	GLuint VAO[20];
	// Element Buffer Object, EBO
	GLuint EBO[20];
	// Vertex Buffer Object, VBO
	GLuint VBO[20];
	// �������
	GLuint texture[20];
	// Shader ������
	Shader shader[20];
	// camera
	Camera* camera;
	// Framebuffer Object // ֡�������
	unsigned int fbo;
	// Renderbuffer Object // ��Ⱦ�������/��Ⱦ����
	unsigned int rbo;
	// ������
	unsigned int texColorBuffer;
	// ��������ͼ
	unsigned int cubeTexture;
	// uniform buffer object
	unsigned int uboMatrices;

	Model ourModel;
	Model planetModel;
	Model rockModel;

	//static const int RockAmount = 5000; // ��ͨ��Ⱦ
	static const int RockAmount = 100000; // ���� glDrawElementsInstanced ��Ⱦ
	glm::mat4 modelMatrices[RockAmount];

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

	DrawSimpleTriangle_24(GLuint screenWidth, GLuint screenHeight) {
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;
		aspect = (GLfloat)screenWidth / (GLfloat)screenHeight;
	}

	virtual void OnInitRender(GLFWwindow* window) {
		glfwSetWindowTitle(window, "DrawSimpleTriangle_24");
		camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), YAW, PITCH, 80.0f);
		//float points[] = {
		//	-0.5f,  0.5f, // ����
		//	 0.5f,  0.5f, // ����
		//	 0.5f, -0.5f, // ����
		//	-0.5f, -0.5f  // ����
		//};
		float quadVertices[] = {
			// λ��          // ��ɫ
			-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
			 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
			-0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

			-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
			 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
			 0.05f,  0.05f,  0.0f, 1.0f, 1.0f
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

		// ������ɫ��
		shader[0] = Shader("../res/Shaders/lesson_13_instancing.vs", "../res/Shaders/lesson_13_instancing.fs"); // ʹ�� glDrawArraysInstanced + uniform ��Ⱦ
		shader[1] = Shader("../res/Shaders/lesson_13_instancing_vertex.vs", "../res/Shaders/lesson_13_instancing.fs"); // ʹ�� glDrawArraysInstanced + ʵ��������
		shader[2] = Shader("../res/Shaders/lesson_13_instancing_model.vs", "../res/Shaders/lesson_13_instancing_model.fs"); // ģ�� ֻ����������������
		shader[3] = Shader("../res/Shaders/lesson_10_cubemaps.vs", "../res/Shaders/lesson_10_cubemaps.fs"); // ��պ�
		shader[4] = Shader("../res/Shaders/lesson_13_instancing_model_optimize.vs", "../res/Shaders/lesson_13_instancing_model.fs"); // ģ�� ʹ�� instanceMatrix
		// ���� VBO
		glGenBuffers(20, VBO);
		// ���� EBO
		glGenBuffers(20, EBO);
		// ���� VAO
		glGenVertexArrays(20, VAO);
		// �����������
		glGenTextures(20, texture);
		// ���ص�ͼ��Ĭ�����·�ת
		stbi_set_flip_vertically_on_load(true);

		{ // ���ö������� // �ĸ���(����ɫ)
			// Vertex Array Object
			glBindVertexArray(VAO[0]);
			// ���ƶ������鵽һ�����㻺���й� OpenGL ʹ��
			glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
			glBindVertexArray(0);
		}

		{ // ���ö������� // �ĸ���(����ɫ)
			const int QuadVerticesCount = 100;
			glm::vec2 translations[QuadVerticesCount];
			int index = 0;
			float offset = 0.1f;
			for (int y = -10; y < 10; y += 2) {
				for (int x = -10; x < 10; x += 2) {
					glm::vec2 translation;
					translation.x = (float)x / 10.0f + offset;
					translation.y = (float)y / 10.0f + offset;
					translations[index++] = translation;
				}
			}
			// Vertex Array Object
			glBindVertexArray(VAO[1]);
			// ���ƶ������鵽һ�����㻺���й� OpenGL ʹ��
			glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
			
			glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glVertexAttribDivisor(2, 1);

			glBindVertexArray(0);
		}

		{ // ��պ�
			glBindVertexArray(VAO[3]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glBindVertexArray(0);
		}

		// ��һ���������, Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ // ľ��
		loadTexture(texture[0], "../res/Texture/container2.png", GL_REPEAT, GL_REPEAT, false);
		// ��һ���������, Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ // �߹�
		loadTexture(texture[1], "../res/Texture/container2_specular.png", GL_REPEAT, GL_REPEAT, false);
		// ��һ���������, Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ // �ݴ�
		loadTexture(texture[2], "../res/Texture/grass.png", GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, false);
		// ��һ���������, Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ // ������
		loadTexture(texture[3], "../res/Texture/blending_transparent_window.png", GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, false);

		stbi_set_flip_vertically_on_load(false);
		//ourModel = Model("../res/nanosuit_reflection/nanosuit.obj");
		planetModel = Model("../res/planet/planet.obj");
		rockModel = Model("../res/rock/rock.obj");
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

		{ // ��ʼ��С���Ǵ�����
			srand(glfwGetTime()); // ��ʼ���������
			float radius = 150.0; // 50.0f
			float offset = 25.0f; // 2.5f
			for (unsigned int i = 0; i < RockAmount; i++)
			{
				glm::mat4 model;
				// 1. λ�ƣ��ֲ��ڰ뾶Ϊ 'radius' ��Բ���ϣ�ƫ�Ƶķ�Χ�� [-offset, offset]
				float angle = (float)i / (float)RockAmount * 360.0f;
				float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
				float x = sin(angle) * radius + displacement;
				displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
				float y = displacement * 0.4f; // �����Ǵ��ĸ߶ȱ�x��z�Ŀ��ҪС
				displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
				float z = cos(angle) * radius + displacement;
				model = glm::translate(model, glm::vec3(x, y, z));

				// 2. ���ţ��� 0.05 �� 0.25f ֮������
				float scale = (rand() % 20) / 100.0f + 0.05;
				model = glm::scale(model, glm::vec3(scale));

				// 3. ��ת������һ�����룩���ѡ�����ת�����������������ת
				float rotAngle = (rand() % 360);
				model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

				// 4. ��ӵ������������
				modelMatrices[i] = model;
			}
		}

		{ // ����С���Ǵ��Ķ��㻺�� VBO
			glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
			glBufferData(GL_ARRAY_BUFFER, RockAmount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
			for (int i = 0; i < rockModel.meshes.size(); i++) {
				unsigned int VAO = rockModel.meshes[i].VAO;
				glBindVertexArray(VAO);
				// ��������
				GLsizei vec4Size = sizeof(glm::vec4);
				glEnableVertexAttribArray(3);
				glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
				glEnableVertexAttribArray(4);
				glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
				glEnableVertexAttribArray(5);
				glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
				glEnableVertexAttribArray(6);
				glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

				glVertexAttribDivisor(3, 1);
				glVertexAttribDivisor(4, 1);
				glVertexAttribDivisor(5, 1);
				glVertexAttribDivisor(6, 1);
				glBindVertexArray(0);
			}
		}

		// ������Ȳ���
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// MSAA
		glEnable(GL_MULTISAMPLE);

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
		//glm::mat4 projection = glm::perspective(camera->Zoom, aspect, 0.1f, 100.0f);
		glm::mat4 projection = glm::perspective(camera->Zoom, aspect, 0.1f, 800.0f); // Ϊ�˿�С���Ǵ�, Զƽ�����Զһ��

		if (false) { // skybox // ���Ȼ��� // ��ر����д��
			glm::mat4 skyboxView = glm::mat4(glm::mat3(camera->GetViewMatrix()));
			//glDisable(GL_CULL_FACE);
			glDepthMask(GL_FALSE);
			shader[3].use();
			shader[3].setMat4("view", skyboxView);
			shader[3].setMat4("projection", projection);
			glBindVertexArray(VAO[3]);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glDepthMask(GL_TRUE);
			//glEnable(GL_CULL_FACE);
		}

		// ʹ�� glDrawArraysInstanced + uniform ��Ⱦ
		if (false) {
			const int QuadVerticesCount = 100;
			glm::vec2 translations[QuadVerticesCount];
			int index = 0;
			float offset = 0.1f;
			for (int y = -10; y < 10; y += 2) {
				for (int x = -10; x < 10; x += 2) {
					glm::vec2 translation;
					translation.x = (float)x / 10.0f + offset;
					translation.y = (float)y / 10.0f + offset;
					translations[index++] = translation;
				}
			}

			shader[0].use();
			for (int i = 0; i < QuadVerticesCount; i++) {
				std::stringstream ss;
				std::string index;
				ss << i;
				index = ss.str();
				shader[0].setVec2(("offsets[" + index + "]").c_str(), translations[i]);
			}
			glBindVertexArray(VAO[0]);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, QuadVerticesCount);
		}

		// ʹ�� glDrawArraysInstanced + ʵ��������
		if (false) {
			const int QuadVerticesCount = 100;
			shader[1].use();
			glBindVertexArray(VAO[1]);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, QuadVerticesCount);
		}

		// С���Ǵ�
		if (false) {
			glEnable(GL_CULL_FACE);
			// ��������
			shader[2].use();
			shader[2].setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));
			shader[2].setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));
			glm::mat4 model;
			model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
			model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
			shader[2].setMat4("model", model);
			planetModel.Draw(shader[2]);
			// ����С����
			for (int i = 0; i < RockAmount; i++) {
				shader[2].setMat4("model", modelMatrices[i]);
				rockModel.Draw(shader[2]);
			}
			glDisable(GL_CULL_FACE);
		}

		// С���Ǵ�(ʹ�� instanceMatrix)
		if (true) {
			glEnable(GL_CULL_FACE);
			// ��������
			shader[2].use();
			shader[2].setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));
			shader[2].setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));
			glm::mat4 model;
			model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
			model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
			shader[2].setMat4("model", model);
			planetModel.Draw(shader[2]);
			// ����С����
			shader[4].use();
			shader[4].setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));
			shader[4].setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));
			rockModel.Draw(shader[4], true, RockAmount);
			glDisable(GL_CULL_FACE);
		}
	}

	void OpenMSAA(bool open) {
		if (open) {
			glEnable(GL_MULTISAMPLE);
		}
		else {
			glDisable(GL_MULTISAMPLE);
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
				OpenMSAA(useSpotLight);
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
