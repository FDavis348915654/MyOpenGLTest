/*
	实例化
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

// 加载图片
unsigned int loadTexture(unsigned int textureID, char const * path, GLint textureWrapS, GLint textureWrapT, bool isSRGB);
// 加载天空盒
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
	// 纹理对象
	GLuint texture[20];
	// Shader 程序类
	Shader shader[20];
	// camera
	Camera* camera;
	// Framebuffer Object // 帧缓冲对象
	unsigned int fbo;
	// Renderbuffer Object // 渲染缓冲对象/渲染附件
	unsigned int rbo;
	// 纹理附件
	unsigned int texColorBuffer;
	// 立方体贴图
	unsigned int cubeTexture;
	// uniform buffer object
	unsigned int uboMatrices;

	Model ourModel;
	Model planetModel;
	Model rockModel;

	//static const int RockAmount = 5000; // 普通渲染
	static const int RockAmount = 100000; // 调用 glDrawElementsInstanced 渲染
	glm::mat4 modelMatrices[RockAmount];

	GLuint screenWidth;
	GLuint screenHeight;

	// 平截头体的宽高比
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
		//	-0.5f,  0.5f, // 左上
		//	 0.5f,  0.5f, // 右上
		//	 0.5f, -0.5f, // 右下
		//	-0.5f, -0.5f  // 左下
		//};
		float quadVertices[] = {
			// 位置          // 颜色
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

		// 编译着色器
		shader[0] = Shader("../res/Shaders/lesson_13_instancing.vs", "../res/Shaders/lesson_13_instancing.fs"); // 使用 glDrawArraysInstanced + uniform 渲染
		shader[1] = Shader("../res/Shaders/lesson_13_instancing_vertex.vs", "../res/Shaders/lesson_13_instancing.fs"); // 使用 glDrawArraysInstanced + 实例化数组
		shader[2] = Shader("../res/Shaders/lesson_13_instancing_model.vs", "../res/Shaders/lesson_13_instancing_model.fs"); // 模型 只绘制了漫反射纹理
		shader[3] = Shader("../res/Shaders/lesson_10_cubemaps.vs", "../res/Shaders/lesson_10_cubemaps.fs"); // 天空盒
		shader[4] = Shader("../res/Shaders/lesson_13_instancing_model_optimize.vs", "../res/Shaders/lesson_13_instancing_model.fs"); // 模型 使用 instanceMatrix
		// 生成 VBO
		glGenBuffers(20, VBO);
		// 创建 EBO
		glGenBuffers(20, EBO);
		// 创建 VAO
		glGenVertexArrays(20, VAO);
		// 生成纹理对象
		glGenTextures(20, texture);
		// 加载的图像默认上下翻转
		stbi_set_flip_vertically_on_load(true);

		{ // 设置顶点属性 // 四个点(带颜色)
			// Vertex Array Object
			glBindVertexArray(VAO[0]);
			// 复制顶点数组到一个顶点缓冲中供 OpenGL 使用
			glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
			glBindVertexArray(0);
		}

		{ // 设置顶点属性 // 四个点(带颜色)
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
			// 复制顶点数组到一个顶点缓冲中供 OpenGL 使用
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

		{ // 天空盒
			glBindVertexArray(VAO[3]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glBindVertexArray(0);
		}

		// 绑定一个纹理对象, 为当前绑定的纹理对象设置环绕、过滤方式 // 木箱
		loadTexture(texture[0], "../res/Texture/container2.png", GL_REPEAT, GL_REPEAT, false);
		// 绑定一个纹理对象, 为当前绑定的纹理对象设置环绕、过滤方式 // 高光
		loadTexture(texture[1], "../res/Texture/container2_specular.png", GL_REPEAT, GL_REPEAT, false);
		// 绑定一个纹理对象, 为当前绑定的纹理对象设置环绕、过滤方式 // 草丛
		loadTexture(texture[2], "../res/Texture/grass.png", GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, false);
		// 绑定一个纹理对象, 为当前绑定的纹理对象设置环绕、过滤方式 // 玻璃窗
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

		{ // 初始化小行星带数据
			srand(glfwGetTime()); // 初始化随机种子
			float radius = 150.0; // 50.0f
			float offset = 25.0f; // 2.5f
			for (unsigned int i = 0; i < RockAmount; i++)
			{
				glm::mat4 model;
				// 1. 位移：分布在半径为 'radius' 的圆形上，偏移的范围是 [-offset, offset]
				float angle = (float)i / (float)RockAmount * 360.0f;
				float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
				float x = sin(angle) * radius + displacement;
				displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
				float y = displacement * 0.4f; // 让行星带的高度比x和z的宽度要小
				displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
				float z = cos(angle) * radius + displacement;
				model = glm::translate(model, glm::vec3(x, y, z));

				// 2. 缩放：在 0.05 和 0.25f 之间缩放
				float scale = (rand() % 20) / 100.0f + 0.05;
				model = glm::scale(model, glm::vec3(scale));

				// 3. 旋转：绕着一个（半）随机选择的旋转轴向量进行随机的旋转
				float rotAngle = (rand() % 360);
				model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

				// 4. 添加到矩阵的数组中
				modelMatrices[i] = model;
			}
		}

		{ // 设置小行星带的顶点缓冲 VBO
			glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
			glBufferData(GL_ARRAY_BUFFER, RockAmount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
			for (int i = 0; i < rockModel.meshes.size(); i++) {
				unsigned int VAO = rockModel.meshes[i].VAO;
				glBindVertexArray(VAO);
				// 顶点属性
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

		// 开启深度测试
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// MSAA
		glEnable(GL_MULTISAMPLE);

		//glEnable(GL_CULL_FACE);

		//glEnable(GL_PROGRAM_POINT_SIZE);
		//glPointSize(10.5f);

		// 线框模式(Wireframe Mode)
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
		glm::mat4 projection = glm::perspective(camera->Zoom, aspect, 0.1f, 800.0f); // 为了看小行星带, 远平面设得远一点

		if (false) { // skybox // 最先绘制 // 需关闭深度写入
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

		// 使用 glDrawArraysInstanced + uniform 渲染
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

		// 使用 glDrawArraysInstanced + 实例化数组
		if (false) {
			const int QuadVerticesCount = 100;
			shader[1].use();
			glBindVertexArray(VAO[1]);
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, QuadVerticesCount);
		}

		// 小行星带
		if (false) {
			glEnable(GL_CULL_FACE);
			// 绘制行星
			shader[2].use();
			shader[2].setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));
			shader[2].setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));
			glm::mat4 model;
			model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
			model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
			shader[2].setMat4("model", model);
			planetModel.Draw(shader[2]);
			// 绘制小行星
			for (int i = 0; i < RockAmount; i++) {
				shader[2].setMat4("model", modelMatrices[i]);
				rockModel.Draw(shader[2]);
			}
			glDisable(GL_CULL_FACE);
		}

		// 小行星带(使用 instanceMatrix)
		if (true) {
			glEnable(GL_CULL_FACE);
			// 绘制行星
			shader[2].use();
			shader[2].setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));
			shader[2].setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));
			glm::mat4 model;
			model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
			model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
			shader[2].setMat4("model", model);
			planetModel.Draw(shader[2]);
			// 绘制小行星
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
		// 加速
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
		float yoffset = lastY - ypos; // 注意这里是相反的，因为y坐标是从底部往顶部依次增大的
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
