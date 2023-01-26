#pragma once
#include <GLFW/glfw3.h>

class SimpleDrawTestBase
{
public:
	virtual void OnInitRender(GLFWwindow* window) {}

	virtual void OnPreRender(float deltaTime) {}

	virtual void OnRender() {}

	virtual void OnOverRender() {}

	virtual void OnKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {}

	virtual void OnProcessInput(GLFWwindow* window) {}

	virtual void OnMouseCallback(GLFWwindow* window, double xpos, double ypos) {}

	virtual void OnScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {}

	virtual void OnFrameBufferSizeChanged(GLFWwindow* window, int width, int height) {}

	virtual bool IsUseMSAA() { return false; }

	virtual bool IsHideMouse() { return true; }
};

