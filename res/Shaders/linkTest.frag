#version 330 core

in vec4 vertextColor; // 从顶点着色器传来的输入变量（名称相同、类型相同）

out vec4 color; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

void main()
{
	color = vertextColor;
}
