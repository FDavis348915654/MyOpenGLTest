#version 330 core

in vec3 ourPos;

out vec4 color; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

void main()
{
	color = vec4(ourPos, 1.0f);
}
