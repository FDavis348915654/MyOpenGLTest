/*
	����
*/
#pragma once
#include <string>
#include <iostream>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "stb_image.h"
#include "SimpleDrawTestBase.h"

class DrawSimpleTriangle_5 : public SimpleDrawTestBase
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
	// �� 4 ��, ʹ��һ��uniform������Ϊmix�����ĵ������������ı���������ɼ��ȣ�ʹ���Ϻ��¼����ı����ӻ�Ц���Ŀɼ���
	GLfloat mixValue = 0.5f;

	virtual void OnInitRender(GLFWwindow* window) {
		//std::cout << "call PreRender()" << std::endl;
		GLfloat vertices[] = {
			//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
				 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
				 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
				-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
				-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ����
		};
		// �� 2 ��, �����ò�ͬ�������Ʒ�ʽ���趨һ����0.0f��2.0f��Χ�ڵģ�������ԭ����0.0f��1.0f���������ꡣ���Կ��ܲ��������ӵĽ������4��Ц��
		GLfloat vertices_1[] = {
			//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
				 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // ����
				 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // ����
				-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
				-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // ����
		};
		// �� 3 ��, �����ھ�����ֻ��ʾ����ͼ����м�һ���֣��޸��������꣬�ﵽ�ܿ������������ص�Ч��������ʹ��GL_NEAREST��������˷�ʽ��������ʾ�ø�����
		const GLfloat minUV = 0.45f;
		const GLfloat maxUV = 0.55f;
		GLfloat vertices_2[] = {
			//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
				 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   maxUV, maxUV,   // ����
				 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   maxUV, minUV,   // ����
				-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   minUV, minUV,   // ����
				-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   minUV, maxUV    // ����
		};
		GLuint indices[] = { // ע��������0��ʼ! 
			0, 1, 3, // ��һ��������
			1, 2, 3  // �ڶ���������
		};

		// ������ɫ��
		Shader myShader("../res/Shaders/test_1_texture.vs", "../res/Shaders/test_1_texture.fs");
		program[0] = myShader.ID;
		Shader myShader_1("../res/Shaders/test_1_texture.vs", "../res/Shaders/test_2_texture.fs"); // ����������
		program[1] = myShader_1.ID;
		// �� 4 ��, ʹ��һ��uniform������Ϊmix�����ĵ������������ı���������ɼ��ȣ�ʹ���Ϻ��¼����ı����ӻ�Ц���Ŀɼ���
		Shader myShader_2("../res/Shaders/test_1_texture.vs", "../res/Shaders/test_3_texture.fs");
		program[2] = myShader_2.ID;
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

		{ // ��һ���������, Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ // ľ��
			glBindTexture(GL_TEXTURE_2D, texture[2]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// ���ز���������
			int width, height, nrChannels;
			const char* fileName = "../res/Texture/container.jpg";
			unsigned char *data = stbi_load(fileName, &width, &height, &nrChannels, 0);
			if (data) {
				std::cout << "load texture success, fileName: " << fileName << std::endl;
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
			glBindTexture(GL_TEXTURE_2D, texture[3]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
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

		{ // ��һ���������, Ϊ��ǰ�󶨵�����������û��ơ����˷�ʽ // Ц��, �����Ʒ�ʽΪ GL_MIRRORED_REPEAT; ������� Ϊ GL_NEAREST
			glBindTexture(GL_TEXTURE_2D, texture[4]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			// �༶��Զ����(Mipmap) ��Сʱ(GL_TEXTURE_MIN_FILTER) ����ʹ�ø���ѡ��
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
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
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			// color
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);
			// uv
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
			glEnableVertexAttribArray(2);
			glBindVertexArray(0);
		}

		{ // ���ö������� // �� 2 ��, �����ò�ͬ�������Ʒ�ʽ���趨һ����0.0f��2.0f��Χ�ڵģ�������ԭ����0.0f��1.0f���������ꡣ���Կ��ܲ��������ӵĽ������4��Ц��
			// Vertex Array Object
			glBindVertexArray(VAO[1]);
			// ���ƶ������鵽һ�����㻺���й� OpenGL ʹ��
			glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_1), vertices_1, GL_STATIC_DRAW);
			// �����������鵽һ�����������й� OpenGL ʹ��
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
			// vertex
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			// color
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);
			// uv
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
			glEnableVertexAttribArray(2);
			glBindVertexArray(0);
		}

		{ // ���ö������� // �� 3 ��, �����ھ�����ֻ��ʾ����ͼ����м�һ���֣��޸��������꣬�ﵽ�ܿ������������ص�Ч��������ʹ��GL_NEAREST��������˷�ʽ��������ʾ�ø�����
			// Vertex Array Object
			glBindVertexArray(VAO[2]);
			// ���ƶ������鵽һ�����㻺���й� OpenGL ʹ��
			glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_2), vertices_2, GL_STATIC_DRAW);
			// �����������鵽һ�����������й� OpenGL ʹ��
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
			// vertex
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			// color
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(1);
			// uv/st
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
			glEnableVertexAttribArray(2);
			glBindVertexArray(0);
		}

		//{ // ���� uniforms ֮ǰ��Ҫ use ��Ӧ����ɫ������, ����֮���ٴ� use ����Ҫ��������
		//	glUseProgram(program[1]);
		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_2D, texture[2]);
		//	glActiveTexture(GL_TEXTURE1);
		//	glBindTexture(GL_TEXTURE_2D, texture[3]);
		//}

		// �߿�ģʽ(Wireframe Mode)
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//int width, height;
		//unsigned char* image = SOIL_load_image("../res/Texture/wall.jpg", &width, &height, 0, SOIL_LOAD_RGB);
		//std::cout << "haha load image " << strlen((char *)image) << std::endl;

		//int width, height, nrChannels;
		//unsigned char *data = stbi_load("../res/Texture/wall.jpg", &width, &height, &nrChannels, 0);
		//std::cout << "haha load image " << strlen((char *)data) << std::endl;
	}

	virtual void OnPreRender(float deltaTime) {
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	virtual void OnRender() {
		//std::cout << "call OnRender()" << std::endl;

		// ��������
		if (false) {
			glUseProgram(program[0]);
			glBindTexture(GL_TEXTURE_2D, texture[0]);
			glBindVertexArray(VAO[0]);
			/*
				��һ������ָ�������ǻ��Ƶ�ģʽ�������glDrawArrays��һ����
				�ڶ������������Ǵ�����ƶ���ĸ�����������6��Ҳ����˵����һ����Ҫ����6�����㡣
				���������������������ͣ�������GL_UNSIGNED_INT��
				���һ�����������ǿ���ָ��EBO�е�ƫ���������ߴ���һ���������飬�������ǵ��㲻��ʹ��������������ʱ�򣩣��������ǻ���������д0��
				glDrawElements�����ӵ�ǰ�󶨵�GL_ELEMENT_ARRAY_BUFFERĿ���EBO�л�ȡ������������ζ������ÿ����Ҫʹ��������Ⱦ����ʱ���������Ӧ��EBO�������е��鷳�����ɶ����������Ҳ����Ԫ�ػ���������󶨡��ڰ�VAOʱ���󶨵����һ��Ԫ�ػ���������洢ΪVAO��Ԫ�ػ���������Ȼ�󣬰󶨵�VAOҲ���Զ��󶨸�EBO��
			*/
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		// ����������
		if (false) {
			glUseProgram(program[1]);
			glActiveTexture(GL_TEXTURE0); // �ڰ�����֮ǰ�ȼ�������Ԫ
			glBindTexture(GL_TEXTURE_2D, texture[0]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture[1]);
			glBindVertexArray(VAO[0]);
			glUniform1i(glGetUniformLocation(program[1], "texture1"), 0);
			glUniform1i(glGetUniformLocation(program[1], "texture2"), 1);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		// �� 2 ��, �����ò�ͬ�������Ʒ�ʽ���趨һ����0.0f��2.0f��Χ�ڵģ�������ԭ����0.0f��1.0f���������ꡣ���Կ��ܲ��������ӵĽ������4��Ц��
		if (false) {
			glUseProgram(program[1]);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture[2]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture[3]);
			glBindVertexArray(VAO[1]);
			glUniform1i(glGetUniformLocation(program[1], "texture1"), 0);
			glUniform1i(glGetUniformLocation(program[1], "texture2"), 1);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		// �� 3 ��, �����ھ�����ֻ��ʾ����ͼ����м�һ���֣��޸��������꣬�ﵽ�ܿ������������ص�Ч��������ʹ��GL_NEAREST��������˷�ʽ��������ʾ�ø�����
		if (false) {
			glUseProgram(program[1]);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture[0]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture[4]);
			glBindVertexArray(VAO[2]);
			glUniform1i(glGetUniformLocation(program[1], "texture1"), 0);
			glUniform1i(glGetUniformLocation(program[1], "texture2"), 1);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		// �� 4 ��, ʹ��һ��uniform������Ϊmix�����ĵ������������ı���������ɼ��ȣ�ʹ���Ϻ��¼����ı����ӻ�Ц���Ŀɼ���
		if (true) {
			glUseProgram(program[2]);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture[0]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture[1]);
			glBindVertexArray(VAO[0]);
			glUniform1i(glGetUniformLocation(program[2], "texture1"), 0);
			glUniform1i(glGetUniformLocation(program[2], "texture2"), 1);
			glUniform1f(glGetUniformLocation(program[2], "mixValue"), mixValue);
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
