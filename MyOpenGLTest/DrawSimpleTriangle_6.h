/*
	�任
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
	// shader ����
	GLuint program[10];
	// ����
	GLuint texture[10];
	// ourColor ��� uniform ������λ��
	GLint vertextColorLocation;
	GLint offsetPosLocation;

	GLfloat mixValue = 0.5f;
	/*glm::mat4 trans;*/

	virtual void OnInitRender(GLFWwindow* window) {
		//std::cout << "call PreRender()" << std::endl;
		GLfloat vertices[] = {
			//     ---- λ�� ----      - �������� -
				 0.5f,  0.5f, 0.0f,   1.0f, 1.0f,   // ����
				 0.5f, -0.5f, 0.0f,   1.0f, 0.0f,   // ����
				-0.5f, -0.5f, 0.0f,   0.0f, 0.0f,   // ����
				-0.5f,  0.5f, 0.0f,   0.0f, 1.0f    // ����
		};
		// ������Щ����
		GLuint indices[] = { // ע��������0��ʼ! 
			0, 1, 3, // ��һ��������
			1, 2, 3  // �ڶ���������
		};

		// ������ɫ��
		Shader myShader("../res/Shaders/test_1_trans.vs", "../res/Shaders/test_1_trans.fs");
		program[0] = myShader.ID;
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

		{ // ��һ���������, Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ // ľ��
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
					��һ������ָ��������Ŀ��(Target)������ΪGL_TEXTURE_2D��ζ�Ż������뵱ǰ�󶨵����������ͬһ��Ŀ���ϵ������κΰ󶨵�GL_TEXTURE_1D��GL_TEXTURE_3D���������ܵ�Ӱ�죩��
					�ڶ�������Ϊ����ָ���༶��Զ����ļ��������ϣ�������ֶ�����ÿ���༶��Զ����ļ���Ļ�������������0��Ҳ���ǻ�������
					��������������OpenGL����ϣ����������Ϊ���ָ�ʽ�����ǵ�ͼ��ֻ��RGBֵ���������Ҳ��������ΪRGBֵ��
					���ĸ��͵���������������յ�����Ŀ�Ⱥ͸߶ȡ�����֮ǰ����ͼ���ʱ�򴢴������ǣ���������ʹ�ö�Ӧ�ı�����
					�¸�����Ӧ�����Ǳ���Ϊ0����ʷ���������⣩��
					���ߵڰ˸�����������Դͼ�ĸ�ʽ���������͡�����ʹ��RGBֵ�������ͼ�񣬲������Ǵ���Ϊchar(byte)���飬���ǽ��ᴫ���Ӧֵ��
					���һ��������������ͼ�����ݡ�
					������glTexImage2Dʱ����ǰ�󶨵��������ͻᱻ����������ͼ��Ȼ����Ŀǰֻ�л�������(Base-level)������ͼ�񱻼����ˣ����Ҫʹ�ö༶��Զ�������Ǳ����ֶ��������в�ͬ��ͼ�񣨲��ϵ����ڶ��������������ߣ�ֱ������������֮�����glGenerateMipmap�����Ϊ��ǰ�󶨵������Զ�����������Ҫ�Ķ༶��Զ����
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

		{ // ��һ���������, Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ // Ц��
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

		//{ // ����ƽ����ϰ�� ����д��, ��������, ������, ����ת, ���λ�ơ� translate * rotate * scale
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

		// �߿�ģʽ(Wireframe Mode)
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	virtual void OnPreRender(float deltaTime) {
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	virtual void OnRender() {
		//std::cout << "call OnRender()" << std::endl;

		// ��������
		if (true) {
			glUseProgram(program[0]);

			// ����һ
			//GLfloat angle = 90.0f;
			//GLfloat scale = 0.5f;
			//glm::mat4 trans;
			////trans = glm::rotate(trans, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
			//trans = glm::rotate(trans, angle, glm::vec3(0.0f, 0.0f, 1.0f)); // ��Щ glm ����ֱ�Ӵ��Ƕȵ�, ����Ҫ�� glm::radians(angle)
			//trans = glm::scale(trans, glm::vec3(scale, scale, scale));

			// ���Զ�
			//GLfloat angle = 15.0f + (GLfloat)glfwGetTime() * 25;
			//glm::mat4 trans;
			//trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
			//trans = glm::rotate(trans, angle, glm::vec3(0.0f, 0.0f, 1.0f)); // ��Щ glm ����ֱ�Ӵ��Ƕȵ�

			// ��ϰ��һ, ʹ��Ӧ���������ϵ����һ���任�����Խ���ı�Ϊ����ת����λ�ơ�����������ʲô����������Ϊʲô�ᷢ������������
			GLfloat angle = 15.0f + (GLfloat)glfwGetTime() * 25;
			glm::mat4 trans;
			trans = glm::rotate(trans, angle, glm::vec3(0.0f, 0.0f, 1.0f));
			//trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
			trans = glm::translate(trans, glm::vec3(0.5f, 0.0f, 0.0f));

			GLuint transformLoc = glGetUniformLocation(program[0], "transform");
			/*
				��һ������������Ӧ�ú���Ϥ�ˣ�����uniform��λ��ֵ��
				�ڶ�����������OpenGL���ǽ�Ҫ���Ͷ��ٸ�����������1��
				����������ѯ�������Ƿ�ϣ�������ǵľ������ת��(Transpose)��Ҳ����˵�������Ǿ�����к��С�OpenGL������ͨ��ʹ��һ���ڲ����󲼾֣�����������(Column-major Ordering)���֡�GLM��Ĭ�ϲ��־������������Բ�����Ҫת�þ���������GL_FALSE��
				���һ�������������ľ������ݣ�����GLM�����ǰ����ǵľ��󴢴�ΪOpenGL��ϣ�����ܵ����֣��������Ҫ����GLM���Դ��ĺ���value_ptr���任��Щ���ݡ�
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

			// ��ϰ���, �����ٴε���glDrawElements�����ڶ������ӣ�ֻʹ�ñ任����ڷ��ڲ�ͬ��λ�á���������ӱ��ڷ��ڴ��ڵ����Ͻǣ����һ᲻�ϵ����ţ���������ת������sin���������������ã�����ע��ʹ��sin����ʱӦ�ø�ֵ�ᵼ�����屻��ת��
			GLfloat scale = sin((GLfloat)glfwGetTime() * 1) + 0.25f;
			trans = glm::mat4(1.0f);
			trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
			trans = glm::scale(trans, glm::vec3(scale, scale, scale));
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		// ÿһ֡���ƽ���ʱȡ����(Ҳ���Բ�ȡ��)
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
