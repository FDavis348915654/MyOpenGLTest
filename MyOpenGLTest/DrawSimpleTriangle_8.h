/*
	�����
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

class DrawSimpleTriangle_8 : public SimpleDrawTestBase
{
public:
	// Vertex Array Object, VAO
	GLuint VAO[10];
	// Element Buffer Object, EBO
	GLuint EBO[10];
	// Vertex Buffer Object, VBO
	GLuint VBO[10];
	// shader ����
	GLuint program[10];
	// ����
	GLuint texture[10];
	// ourColor ��� uniform ������λ��
	GLint vertextColorLocation;
	GLint offsetPosLocation;
	GLuint screenWidth;
	GLuint screenHeight;

	GLfloat fov = 45.0f;
	GLfloat aspect = 1;

	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;

	// ƫ����
	GLfloat yaw = 0.0f;
	// ������
	GLfloat pitch = 0.0f;
	// ��ת��
	GLfloat roll = 0.0f;

	GLfloat lastX = 400.0f;
	GLfloat lastY = 300.0f;
	GLboolean firstMouse = true;

	DrawSimpleTriangle_8(GLuint screenWidth, GLuint screenHeight) {
		this->screenWidth = screenWidth;
		this->screenHeight = screenHeight;
		aspect = (GLfloat)screenWidth / (GLfloat)screenHeight;
		cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	}

	virtual void OnInitRender(GLFWwindow* window) {
		//std::cout << "call PreRender()" << std::endl;
		GLfloat vertices[] = {
			//     ---- λ�� ----      - �������� -
				 0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // ����
				 0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // ����
				-0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // ����
				-0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // ����
		};
		float verticesBox[] = {
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
		// ������Щ����
		GLuint indices[] = { // ע��������0��ʼ! 
			0, 1, 3, // ��һ��������
			1, 2, 3  // �ڶ���������
		};

		// ������ɫ��
		Shader myShader("../res/Shaders/test_1_coordinate.vs", "../res/Shaders/test_1_trans.frag");
		program[0] = myShader.program;
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

		{ // ��һ����������, Ϊ��ǰ�󶨵������������û��ơ����˷�ʽ // ľ��
			glBindTexture(GL_TEXTURE_2D, texture[0]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// ���ز���������
			int width, height, nrChannels;
			const char* fileName = "../res/Texture/container.jpg";
			unsigned char *data = stbi_load(fileName, &width, &height, &nrChannels, 0);
			if (data) {
				std::cout << "load texture success, fileName: " << fileName << std::endl;
				/*
				�����ܳ�������Ҳ���٣���������һ��һ���ؽ��⣺
					��һ������ָ��������Ŀ��(Target)������ΪGL_TEXTURE_2D��ζ�Ż������뵱ǰ�󶨵�����������ͬһ��Ŀ���ϵ��������κΰ󶨵�GL_TEXTURE_1D��GL_TEXTURE_3D�����������ܵ�Ӱ�죩��
					�ڶ�������Ϊ����ָ���༶��Զ�����ļ��������ϣ�������ֶ�����ÿ���༶��Զ�����ļ���Ļ�������������0��Ҳ���ǻ�������
					��������������OpenGL����ϣ������������Ϊ���ָ�ʽ�����ǵ�ͼ��ֻ��RGBֵ���������Ҳ����������ΪRGBֵ��
					���ĸ��͵���������������յ������Ŀ��Ⱥ͸߶ȡ�����֮ǰ����ͼ���ʱ�򴢴������ǣ���������ʹ�ö�Ӧ�ı�����
					�¸�����Ӧ�����Ǳ���Ϊ0����ʷ���������⣩��
					���ߵڰ˸�����������Դͼ�ĸ�ʽ���������͡�����ʹ��RGBֵ�������ͼ�񣬲������Ǵ���Ϊchar(byte)���飬���ǽ��ᴫ���Ӧֵ��
					���һ��������������ͼ�����ݡ�
					������glTexImage2Dʱ����ǰ�󶨵���������ͻᱻ����������ͼ��Ȼ����Ŀǰֻ�л�������(Base-level)������ͼ�񱻼����ˣ����Ҫʹ�ö༶��Զ���������Ǳ����ֶ��������в�ͬ��ͼ�񣨲��ϵ����ڶ��������������ߣ�ֱ������������֮�����glGenerateMipmap�����Ϊ��ǰ�󶨵������Զ�����������Ҫ�Ķ༶��Զ������
				*/
				// ��������
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else {
				std::cout << "error, Failed to load texture, fileName: " << fileName << std::endl;
			}
			stbi_image_free(data);
		}

		{ // ��һ����������, Ϊ��ǰ�󶨵������������û��ơ����˷�ʽ // Ц��
			glBindTexture(GL_TEXTURE_2D, texture[1]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// ���ز���������
			int width, height, nrChannels;
			const char* fileName = "../res/Texture/awesomeface.png";
			unsigned char *data = stbi_load(fileName, &width, &height, &nrChannels, 0);
			if (data) {
				std::cout << "load texture success, fileName: " << fileName << std::endl;
				// ��������
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else {
				std::cout << "error, Failed to load texture, fileName: " << fileName << std::endl;
			}
			stbi_image_free(data);
		}

		{ // ���ö�������
			// Vertex Array Object
			glBindVertexArray(VAO[0]);
			// ���ƶ������鵽һ�����㻺���й� OpenGL ʹ��
			glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			// �����������鵽һ�����������й� OpenGL ʹ��
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

		{ // ���ö�������
			// Vertex Array Object
			glBindVertexArray(VAO[1]);
			// ���ƶ������鵽һ�����㻺���й� OpenGL ʹ��
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

		// ������Ȳ���
		glEnable(GL_DEPTH_TEST);
	}

	virtual void OnPreRender(float deltaTime) {
		// Clear the colorbuffer
		if (false) {
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		// z ����
		if (true) {
			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
	}

	virtual void OnRender() {
		//std::cout << "call OnRender()" << std::endl;

		// �������� box // ���Χ�Ƴ���ת��
		if (false) {
			glUseProgram(program[0]);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture[0]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture[1]);
			glUniform1i(glGetUniformLocation(program[0], "texture1"), 0);
			glUniform1i(glGetUniformLocation(program[0], "texture2"), 1);
			glBindVertexArray(VAO[1]);

			GLuint modelLoc = glGetUniformLocation(program[0], "model");
			GLuint viewLoc = glGetUniformLocation(program[0], "view");
			GLuint projectionLoc = glGetUniformLocation(program[0], "projection");
			GLfloat radius = 10.0f;
			GLfloat camX = (float)sin(glfwGetTime()) * radius;
			GLfloat camY = (float)cos(glfwGetTime()) * radius;
			glm::mat4 view;
			view = glm::lookAt(glm::vec3(camX, 0.0f, camY), glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 projection;
			projection = glm::perspective(fov, aspect, 0.1f, 100.0f);
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

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
				glm::mat4 model;
				model = glm::translate(model, cubePositions[i]);
				GLfloat angle = 20.0f * i;
				GLfloat step = 0.0f;
				if (0 == i % 3) {
					step = (GLfloat)glfwGetTime() * 50.0f;
				}
				model = glm::rotate(model, step + angle, glm::vec3(1.0f, 0.3f, 0.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}

		// �������� box // ��������ƶ�
		if (true) {
			glUseProgram(program[0]);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture[0]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture[1]);
			glUniform1i(glGetUniformLocation(program[0], "texture1"), 0);
			glUniform1i(glGetUniformLocation(program[0], "texture2"), 1);
			glBindVertexArray(VAO[1]);

			GLuint modelLoc = glGetUniformLocation(program[0], "model");
			GLuint viewLoc = glGetUniformLocation(program[0], "view");
			GLuint projectionLoc = glGetUniformLocation(program[0], "projection");
			GLfloat radius = 10.0f;
			glm::mat4 view;
			view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
			glm::mat4 projection;
			projection = glm::perspective(fov, aspect, 0.1f, 100.0f);
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

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
				glm::mat4 model;
				model = glm::translate(model, cubePositions[i]);
				GLfloat angle = 20.0f * i;
				GLfloat step = 0.0f;
				if (0 == i % 3) {
					step = (GLfloat)glfwGetTime() * 50.0f;
				}
				model = glm::rotate(model, step + angle, glm::vec3(1.0f, 0.3f, 0.5f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}

		// ÿһ֡���ƽ���ʱȡ����(Ҳ���Բ�ȡ��)
		glBindVertexArray(0);
	}

	virtual void OnOverRender() {
		/*	glDeleteVertexArrays(3, VAO);
			glDeleteBuffers(3, VBO);*/
	}

	virtual void OnProcessInput(GLFWwindow* window) {
		GLfloat cameraSpeed = 0.05f;
		// ����
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			cameraSpeed *= 2.0f;
		}

		// �������
		if (false) {
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

		// fps ��� // ��ϰ��һ, �������Ƿ��ܹ��޸�������࣬ʹ�����ܹ����һ��������FPS�������Ҳ����˵���ܹ�������У�����ֻ�ܹ�����xzƽ����
		if (true) {
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
		printf("screenPos(%f, %f)\n", xpos, ypos);
		if (firstMouse) {
			lastX = (float)xpos;
			lastY = (float)ypos;
			firstMouse = false;
			return;
		}

		float xoffset = (float)xpos - lastX;
		float yoffset = lastY - (float)ypos; // ע���������෴�ģ���Ϊy�����Ǵӵײ����������������
		lastX = (float)xpos;
		lastY = (float)ypos;

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

	// ��ϰ���, ��׼�𰸡����Ŵ������Լ���LookAt��������������Ҫ�ֶ�����һ��������һ��ʼ���۵Ĺ۲��������ĺ���ʵ�����滻GLM��LookAt�������������Ƿ���һ���ع�����
	// ps: ֻ��˵���������е�Ҫ��, ��ľ��Ǹ�����ֵ, �һ���Ϊ�� translate * roll * pitch * yaw * model, ��ʱ��һ��Ҫ��һ���������
	// Custom implementation of the LookAt function
	glm::mat4 calculate_lookAt_matrix(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp)
	{
		// 1. Position = known
		// 2. Calculate cameraDirection
		glm::vec3 zaxis = glm::normalize(position - target);
		// 3. Get positive right axis vector
		glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize(worldUp), zaxis));
		// 4. Calculate camera up vector
		glm::vec3 yaxis = glm::cross(zaxis, xaxis);

		// Create translation and rotation matrix
		// In glm we access elements as mat[col][row] due to column-major layout
		glm::mat4 translation = glm::mat4(1.0f); // Identity matrix by default
		translation[3][0] = -position.x; // Third column, first row
		translation[3][1] = -position.y;
		translation[3][2] = -position.z;
		glm::mat4 rotation = glm::mat4(1.0f);
		rotation[0][0] = xaxis.x; // First column, first row
		rotation[1][0] = xaxis.y;
		rotation[2][0] = xaxis.z;
		rotation[0][1] = yaxis.x; // First column, second row
		rotation[1][1] = yaxis.y;
		rotation[2][1] = yaxis.z;
		rotation[0][2] = zaxis.x; // First column, third row
		rotation[1][2] = zaxis.y;
		rotation[2][2] = zaxis.z;

		// Return lookAt matrix as combination of translation and rotation matrix
		return rotation * translation; // Remember to read from right to left (first translation then rotation)
	}

	//// Don't forget to replace glm::lookAt with your own version
	//// view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//view = calculate_lookAt_matrix(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
};