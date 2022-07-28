#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include "Shader.h"

// test
//#include "../MyOpenGLTest/DrawSimpleTriangle.h"
//#include "../MyOpenGLTest/DrawSimpleTriangle_2.h"
//#include "../MyOpenGLTest/DrawSimpleTriangle_3.h"
//#include "../MyOpenGLTest/DrawSimpleTriangle_4.h"
//#include "../MyOpenGLTest/DrawSimpleTriangle_5.h"
//#include "../MyOpenGLTest/DrawSimpleTriangle_6.h"
//#include "../MyOpenGLTest/DrawSimpleTriangle_7.h"
//#include "../MyOpenGLTest/DrawSimpleTriangle_8.h"
//#include "../MyOpenGLTest/DrawSimpleTriangle_9.h"
//#include "../MyOpenGLTest/DrawSimpleTriangle_10.h"
//#include "../MyOpenGLTest/DrawSimpleTriangle_11.h"
//#include "../MyOpenGLTest/DrawSimpleTriangle_12.h"
#include "../MyOpenGLTest/DrawSimpleTriangle_13.h"
#include "../MyOpenGLTest/DrawSimpleTriangle_14.h"

// Function prototypes
// �����ص�
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
// ���ص�
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// ���ֻص�
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// ��Ļ�ߴ緢���仯
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// ��ӡ������������
void ShowMaxVertex();
// ����ͼƬ
unsigned int loadTexture(unsigned int textureID, char const * path);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

//DrawSimpleTriangle obj;
//DrawSimpleTriangle_2 obj;
//DrawSimpleTriangle_3 obj;
//DrawSimpleTriangle_4 obj;
//DrawSimpleTriangle_5 obj;
//DrawSimpleTriangle_6 obj;
//DrawSimpleTriangle_7 obj(WIDTH, HEIGHT);
//DrawSimpleTriangle_8 obj(WIDTH, HEIGHT);
//DrawSimpleTriangle_9 obj(WIDTH, HEIGHT);
//DrawSimpleTriangle_10 obj(WIDTH, HEIGHT);
//DrawSimpleTriangle_11 obj(WIDTH, HEIGHT);
//DrawSimpleTriangle_12 obj(WIDTH, HEIGHT);
//DrawSimpleTriangle_13 obj(WIDTH, HEIGHT);
DrawSimpleTriangle_14 obj(WIDTH, HEIGHT);

// The MAIN function, from here we start the application and run the game loop
int main()
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	// ��ӡ������������
	ShowMaxVertex();
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

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
	// ��ǰ�����ڽ�����ʱ�������
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
unsigned int loadTexture(unsigned int textureID, char const * path)
{
	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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

// ��ӡ������������
void ShowMaxVertex()
{
	GLint nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	printf("Maximun nr of vertext attributes supported: %d\n", nrAttributes);
}