#version 330 core

out vec4 color;

uniform vec4 outColor; // 在 OpenGL 程序代码中设定这个变量

void main()
{
	color = outColor;
}
