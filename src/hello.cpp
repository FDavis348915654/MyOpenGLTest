#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include "Shader.h"

// test
#include "../MyOpenGLTest/DrawSimpleTriangle.h" // 你好，三角形。VBO
#include "../MyOpenGLTest/DrawSimpleTriangle_2.h" // 你好，三角形。EBO
#include "../MyOpenGLTest/DrawSimpleTriangle_3.h" // 你好，三角形
#include "../MyOpenGLTest/DrawSimpleTriangle_4.h" // 着色器
#include "../MyOpenGLTest/DrawSimpleTriangle_5.h" // 纹理
#include "../MyOpenGLTest/DrawSimpleTriangle_6.h" // 变换
#include "../MyOpenGLTest/DrawSimpleTriangle_7.h" // 坐标系统
#include "../MyOpenGLTest/DrawSimpleTriangle_8.h" // 摄像机
#include "../MyOpenGLTest/DrawSimpleTriangle_9.h" // 颜色
#include "../MyOpenGLTest/DrawSimpleTriangle_10.h" // 基础光照
#include "../MyOpenGLTest/DrawSimpleTriangle_11.h" // 材质
#include "../MyOpenGLTest/DrawSimpleTriangle_12.h" // 光照贴图
#include "../MyOpenGLTest/DrawSimpleTriangle_13.h" // 投光物
#include "../MyOpenGLTest/DrawSimpleTriangle_14.h" // 多光源
#include "../MyOpenGLTest/DrawSimpleTriangle_15.h" // 模型显示
#include "../MyOpenGLTest/DrawSimpleTriangle_16.h" // 深度测试
#include "../MyOpenGLTest/DrawSimpleTriangle_17.h" // 模板测试
#include "../MyOpenGLTest/DrawSimpleTriangle_18.h" // 混合
#include "../MyOpenGLTest/DrawSimpleTriangle_19.h" // 面剔除
#include "../MyOpenGLTest/DrawSimpleTriangle_20.h" // 帧缓冲
#include "../MyOpenGLTest/DrawSimpleTriangle_21.h" // 立方体贴图
#include "../MyOpenGLTest/DrawSimpleTriangle_22.h" // 高级 GLSL
#include "../MyOpenGLTest/DrawSimpleTriangle_23.h" // 几何着色器
#include "../MyOpenGLTest/DrawSimpleTriangle_24.h" // 实例化
#include "../MyOpenGLTest/DrawSimpleTriangle_25.h" // 抗锯齿
#include "../MyOpenGLTest/DrawSimpleTriangle_26.h" // 高级光照
#include "../MyOpenGLTest/DrawSimpleTriangle_27.h" // Gamma校正
#include "../MyOpenGLTest/DrawSimpleTriangle_28.h" // 阴影映射
#include "../MyOpenGLTest/DrawSimpleTriangle_29.h" // 点阴影

// Function prototypes
// 按键回调
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
// 鼠标回调
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// 滚轮回调
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// 屏幕尺寸发生变化
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// 打印顶点属性上限
void ShowMaxVertex();
// 加载图片
unsigned int loadTexture(unsigned int textureID, char const * path, GLint textureWrapS, GLint textureWrapT, bool isSRGB);
// 加载天空盒
unsigned int loadCubemap(std::vector<std::string> faces);

// Window dimensions
//const GLuint WIDTH = 800, HEIGHT = 600;
const GLuint WIDTH = 800 * 2, HEIGHT = 600 * 2;
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//DrawSimpleTriangle obj(WIDTH, HEIGHT); // 你好，三角形。VBO
//DrawSimpleTriangle_2 obj(WIDTH, HEIGHT); // 你好，三角形。EBO
//DrawSimpleTriangle_3 obj(WIDTH, HEIGHT); // 你好，三角形
//DrawSimpleTriangle_4 obj(WIDTH, HEIGHT); // 着色器
//DrawSimpleTriangle_5 obj(WIDTH, HEIGHT); // 纹理
//DrawSimpleTriangle_6 obj(WIDTH, HEIGHT); // 变换
//DrawSimpleTriangle_7 obj(WIDTH, HEIGHT); // 坐标系统
//DrawSimpleTriangle_8 obj(WIDTH, HEIGHT); // 摄像机
//DrawSimpleTriangle_9 obj(WIDTH, HEIGHT); // 颜色
//DrawSimpleTriangle_10 obj(WIDTH, HEIGHT); // 基础光照
//DrawSimpleTriangle_11 obj(WIDTH, HEIGHT); // 材质
//DrawSimpleTriangle_12 obj(WIDTH, HEIGHT); // 光照贴图
//DrawSimpleTriangle_13 obj(WIDTH, HEIGHT); // 投光物
//DrawSimpleTriangle_14 obj(WIDTH, HEIGHT); // 多光源
//DrawSimpleTriangle_15 obj(WIDTH, HEIGHT); // 模型显示
//DrawSimpleTriangle_16 obj(WIDTH, HEIGHT); // 深度测试
//DrawSimpleTriangle_17 obj(WIDTH, HEIGHT); // 模板测试
//DrawSimpleTriangle_18 obj(WIDTH, HEIGHT); // 混合
//DrawSimpleTriangle_19 obj(WIDTH, HEIGHT); // 面剔除
//DrawSimpleTriangle_20 obj(WIDTH, HEIGHT); // 帧缓冲
//DrawSimpleTriangle_21 obj(WIDTH, HEIGHT); // 立方体贴图
//DrawSimpleTriangle_22 obj(WIDTH, HEIGHT); // 高级 GLSL
//DrawSimpleTriangle_23 obj(WIDTH, HEIGHT); // 几何着色器
//DrawSimpleTriangle_24 obj(WIDTH, HEIGHT); // 实例化
//DrawSimpleTriangle_25 obj(WIDTH, HEIGHT); // 抗锯齿
//DrawSimpleTriangle_26 obj(WIDTH, HEIGHT); // 高级光照
//DrawSimpleTriangle_27 obj(WIDTH, HEIGHT); // Gamma校正
//DrawSimpleTriangle_28 obj(WIDTH, HEIGHT); // 阴影映射
DrawSimpleTriangle_29 obj(WIDTH, HEIGHT); // 点阴影

// The MAIN function, from here we start the application and run the game loop
int main()
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	// 打印顶点属性上限
	ShowMaxVertex();
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	// MSAA
	if (obj.IsUseMSAA()) {
		glfwWindowHint(GLFW_SAMPLES, 4);
	}

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);

	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// 当前窗口在焦点上时隐藏鼠标
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Define the viewport dimensions
	int width, height;

	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	obj.OnInitRender(window);
	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		obj.OnPreRender(deltaTime);

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		obj.OnProcessInput(window);

		obj.OnRender();

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	obj.OnOverRender();
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//printf("key_callback, key: %d, scancode: %d, action: %d, mode: %d\n", key, scancode, action, mode);
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}
	obj.OnKeyCallback(window, key, scancode, action, mode);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	obj.OnMouseCallback(window, xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	obj.OnScrollCallback(window, xoffset, yoffset);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	obj.OnFrameBufferSizeChanged(window, width, height);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(unsigned int textureID, char const * path, GLint textureWrapS, GLint textureWrapT, bool isSRGB)
{
	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum internalFormat;
		GLenum dataFormat;
		if (nrComponents == 1) {
			internalFormat = GL_RED;
			dataFormat = GL_RED;
		}
		else if (nrComponents == 3) {
			internalFormat = isSRGB ? GL_SRGB : GL_RGB;
			dataFormat = GL_RGB;
		}
		else if (nrComponents == 4) {
			internalFormat = isSRGB ? GL_SRGB_ALPHA : GL_RGBA;
			dataFormat = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrapT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

unsigned int loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++) {
		// 读取图像
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else {
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

// 打印顶点属性上限
void ShowMaxVertex()
{
	GLint nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	printf("Maximun nr of vertext attributes supported: %d\n", nrAttributes);
}
