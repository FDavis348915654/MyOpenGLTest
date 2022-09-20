/*
	你好，三角形。EBO
*/
#pragma once
#include <iostream>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include "../src/Shader.h"
#include "SimpleDrawTestBase.h"

class DrawSimpleTriangle_2 : public SimpleDrawTestBase
{
public:
	// Vertex Array Object, VAO
	GLuint VAO;
	// shader 程序
	GLuint program;

	virtual void OnInitRender(GLFWwindow* window) {
		//std::cout << "call PreRender()" << std::endl;
		GLfloat vertices[] = {
			0.5f, 0.5f, 0.0f,   // 右上角
			0.5f, -0.5f, 0.0f,  // 右下角
			-0.5f, -0.5f, 0.0f, // 左下角
			-0.5f, 0.5f, 0.0f   // 左上角
		};
		GLuint indices[] = { // 注意索引从0开始! 
			0, 1, 3, // 第一个三角形
			1, 2, 3  // 第二个三角形
		};
		// 编译着色器
		Shader myShader("../res/Shaders/default.vs", "../res/Shaders/default.fs");
		program = myShader.ID;
		// 生成 VBO
		GLuint VBO;
		glGenBuffers(1, &VBO);
		// 生成 EBO
		GLuint EBO;
		glGenBuffers(1, &EBO);
		// 创建 VAO
		glGenVertexArrays(1, &VAO);
		// 绑定 VAO
		glBindVertexArray(VAO);
		{
			// 复制顶点数组到一个顶点缓冲中供 OpenGL 使用
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			// 复制索引数组到一个索引缓冲中供 OpenGL 使用
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
			// 设置顶点属性指针
			/*
				第一个参数指定我们要配置的顶点属性。还记得我们在顶点着色器中使用layout(location = 0)定义了position顶点属性的位置值(Location)吗？它可以把顶点属性的位置值设置为0。因为我们希望把数据传递到这一个顶点属性中，所以这里我们传入0。
				第二个参数指定顶点属性的大小。顶点属性是一个vec3，它由3个值组成，所以大小是3。
				第三个参数指定数据的类型，这里是GL_FLOAT(GLSL中vec*都是由浮点数值组成的)。
				下个参数定义我们是否希望数据被标准化(Normalize)。如果我们设置为GL_TRUE，所有数据都会被映射到0（对于有符号型signed数据是 - 1）到1之间。我们把它设置为GL_FALSE。
				第五个参数叫做步长(Stride)，它告诉我们在连续的顶点属性组之间的间隔。由于下个组位置数据在3个GLfloat之后，我们把步长设置为3 * sizeof(GLfloat)。要注意的是由于我们知道这个数组是紧密排列的（在两个顶点属性之间没有空隙）我们也可以设置为0来让OpenGL决定具体步长是多少（只有当数值是紧密排列时才可用）。一旦我们有更多的顶点属性，我们就必须更小心地定义每个顶点属性之间的间隔，我们在后面会看到更多的例子(译注: 这个参数的意思简单说就是从这个属性第二次出现的地方到整个数组0位置之间有多少字节)。
				最后一个参数的类型是GLvoid*，所以需要我们进行这个奇怪的强制类型转换。它表示位置数据在缓冲中起始位置的偏移量(Offset)。由于位置数据在数组的开头，所以这里是0。我们会在后面详细解释这个参数。
			*/
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
		}
		// 解绑 VAO
		glBindVertexArray(0);

		// 线框模式(Wireframe Mode)
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	virtual void OnPreRender(float deltaTime) {
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	virtual void OnRender() {
		//std::cout << "call OnRender()" << std::endl;
		glUseProgram(program);
		glBindVertexArray(VAO);
		/*
			第一个参数指定了我们绘制的模式，这个和glDrawArrays的一样。
			第二个参数是我们打算绘制顶点的个数，这里填6，也就是说我们一共需要绘制6个顶点。
			第三个参数是索引的类型，这里是GL_UNSIGNED_INT。
			最后一个参数里我们可以指定EBO中的偏移量（或者传递一个索引数组，但是这是当你不在使用索引缓冲对象的时候），但是我们会在这里填写0。
			glDrawElements函数从当前绑定到GL_ELEMENT_ARRAY_BUFFER目标的EBO中获取其索引。这意味着我们每次想要使用索引渲染对象时都必须绑定相应的EBO，这又有点麻烦。碰巧顶点数组对象也跟踪元素缓冲区对象绑定。在绑定VAO时，绑定的最后一个元素缓冲区对象存储为VAO的元素缓冲区对象。然后，绑定到VAO也会自动绑定该EBO。
		*/
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawElements(GL_LINE_LOOP, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	virtual void OnOverRender() {

	}
};
