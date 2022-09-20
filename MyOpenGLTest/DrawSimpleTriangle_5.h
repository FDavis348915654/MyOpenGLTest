/*
	纹理
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
	// shader 程序
	GLuint program[10];
	// 纹理
	GLuint texture[10];
	// ourColor 这个 uniform 变量的位置
	GLint vertextColorLocation;
	GLint offsetPosLocation;
	// 第 4 题, 使用一个uniform变量作为mix函数的第三个参数来改变两个纹理可见度，使用上和下键来改变箱子或笑脸的可见度
	GLfloat mixValue = 0.5f;

	virtual void OnInitRender(GLFWwindow* window) {
		//std::cout << "call PreRender()" << std::endl;
		GLfloat vertices[] = {
			//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
				 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
				 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
				-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
				-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
		};
		// 第 2 题, 尝试用不同的纹理环绕方式，设定一个从0.0f到2.0f范围内的（而不是原来的0.0f到1.0f）纹理坐标。试试看能不能在箱子的角落放置4个笑脸
		GLfloat vertices_1[] = {
			//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
				 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // 右上
				 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // 右下
				-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
				-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // 左上
		};
		// 第 3 题, 尝试在矩形上只显示纹理图像的中间一部分，修改纹理坐标，达到能看见单个的像素的效果。尝试使用GL_NEAREST的纹理过滤方式让像素显示得更清晰
		const GLfloat minUV = 0.45f;
		const GLfloat maxUV = 0.55f;
		GLfloat vertices_2[] = {
			//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
				 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   maxUV, maxUV,   // 右上
				 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   maxUV, minUV,   // 右下
				-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   minUV, minUV,   // 左下
				-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   minUV, maxUV    // 左上
		};
		GLuint indices[] = { // 注意索引从0开始! 
			0, 1, 3, // 第一个三角形
			1, 2, 3  // 第二个三角形
		};

		// 编译着色器
		Shader myShader("../res/Shaders/test_1_texture.vs", "../res/Shaders/test_1_texture.fs");
		program[0] = myShader.ID;
		Shader myShader_1("../res/Shaders/test_1_texture.vs", "../res/Shaders/test_2_texture.fs"); // 两个纹理混合
		program[1] = myShader_1.ID;
		// 第 4 题, 使用一个uniform变量作为mix函数的第三个参数来改变两个纹理可见度，使用上和下键来改变箱子或笑脸的可见度
		Shader myShader_2("../res/Shaders/test_1_texture.vs", "../res/Shaders/test_3_texture.fs");
		program[2] = myShader_2.ID;
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

		{ // 绑定一个纹理对象, 为当前绑定的纹理对象设置环绕、过滤方式 // 木箱
			glBindTexture(GL_TEXTURE_2D, texture[2]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// 加载并生成纹理
			int width, height, nrChannels;
			const char* fileName = "../res/Texture/container.jpg";
			unsigned char *data = stbi_load(fileName, &width, &height, &nrChannels, 0);
			if (data) {
				std::cout << "load texture success, fileName: " << fileName << std::endl;
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
			glBindTexture(GL_TEXTURE_2D, texture[3]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
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

		{ // 绑定一个纹理对象, 为当前绑定的纹理对象设置环绕、过滤方式 // 笑脸, 纹理环绕方式为 GL_MIRRORED_REPEAT; 纹理过滤 为 GL_NEAREST
			glBindTexture(GL_TEXTURE_2D, texture[4]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
			// 多级渐远纹理(Mipmap) 缩小时(GL_TEXTURE_MIN_FILTER) 可以使用更多选项
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
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

		{ // 设置顶点属性 // 第 2 题, 尝试用不同的纹理环绕方式，设定一个从0.0f到2.0f范围内的（而不是原来的0.0f到1.0f）纹理坐标。试试看能不能在箱子的角落放置4个笑脸
			// Vertex Array Object
			glBindVertexArray(VAO[1]);
			// 复制顶点数组到一个顶点缓冲中供 OpenGL 使用
			glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_1), vertices_1, GL_STATIC_DRAW);
			// 复制索引数组到一个索引缓冲中供 OpenGL 使用
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

		{ // 设置顶点属性 // 第 3 题, 尝试在矩形上只显示纹理图像的中间一部分，修改纹理坐标，达到能看见单个的像素的效果。尝试使用GL_NEAREST的纹理过滤方式让像素显示得更清晰
			// Vertex Array Object
			glBindVertexArray(VAO[2]);
			// 复制顶点数组到一个顶点缓冲中供 OpenGL 使用
			glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_2), vertices_2, GL_STATIC_DRAW);
			// 复制索引数组到一个索引缓冲中供 OpenGL 使用
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

		//{ // 设置 uniforms 之前需要 use 对应的着色器程序, 设置之后再次 use 不需要重新设置
		//	glUseProgram(program[1]);
		//	glActiveTexture(GL_TEXTURE0);
		//	glBindTexture(GL_TEXTURE_2D, texture[2]);
		//	glActiveTexture(GL_TEXTURE1);
		//	glBindTexture(GL_TEXTURE_2D, texture[3]);
		//}

		// 线框模式(Wireframe Mode)
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

		// 绘制纹理
		if (false) {
			glUseProgram(program[0]);
			glBindTexture(GL_TEXTURE_2D, texture[0]);
			glBindVertexArray(VAO[0]);
			/*
				第一个参数指定了我们绘制的模式，这个和glDrawArrays的一样。
				第二个参数是我们打算绘制顶点的个数，这里填6，也就是说我们一共需要绘制6个顶点。
				第三个参数是索引的类型，这里是GL_UNSIGNED_INT。
				最后一个参数里我们可以指定EBO中的偏移量（或者传递一个索引数组，但是这是当你不在使用索引缓冲对象的时候），但是我们会在这里填写0。
				glDrawElements函数从当前绑定到GL_ELEMENT_ARRAY_BUFFER目标的EBO中获取其索引。这意味着我们每次想要使用索引渲染对象时都必须绑定相应的EBO，这又有点麻烦。碰巧顶点数组对象也跟踪元素缓冲区对象绑定。在绑定VAO时，绑定的最后一个元素缓冲区对象存储为VAO的元素缓冲区对象。然后，绑定到VAO也会自动绑定该EBO。
			*/
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		// 两个纹理混合
		if (false) {
			glUseProgram(program[1]);
			glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活纹理单元
			glBindTexture(GL_TEXTURE_2D, texture[0]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture[1]);
			glBindVertexArray(VAO[0]);
			glUniform1i(glGetUniformLocation(program[1], "texture1"), 0);
			glUniform1i(glGetUniformLocation(program[1], "texture2"), 1);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		// 第 2 题, 尝试用不同的纹理环绕方式，设定一个从0.0f到2.0f范围内的（而不是原来的0.0f到1.0f）纹理坐标。试试看能不能在箱子的角落放置4个笑脸
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

		// 第 3 题, 尝试在矩形上只显示纹理图像的中间一部分，修改纹理坐标，达到能看见单个的像素的效果。尝试使用GL_NEAREST的纹理过滤方式让像素显示得更清晰
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

		// 第 4 题, 使用一个uniform变量作为mix函数的第三个参数来改变两个纹理可见度，使用上和下键来改变箱子或笑脸的可见度
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
