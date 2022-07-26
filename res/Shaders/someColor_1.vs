#version 330 core

layout (location = 0) in vec3 position; // position 变量的属性位置值为 0
layout (location = 1) in vec3 color; // color 变量的属性位置值为 1

out vec3 ourColor; // 为片段着色器指定一个颜色输出

//uniform vec3 offsetPos; // 可以直接使用 float, 效果更佳
uniform float offsetX;

void main()
{
	//gl_Position = vec4(position + offsetPos, 1.0); // 注意我们如何把一个 vec3 作为 vec4 的构造器的参数
	gl_Position = vec4(position.x + offsetX, position.yz, 1.0); // 注意我们如何把一个 vec3 作为 vec4 的构造器的参数
	ourColor = color;
}
