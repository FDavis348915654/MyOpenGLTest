#version 330 core

layout (location = 0) in vec3 position; // position 变量的属性位置值为 0

out vec4 vertextColor; // 为片段着色器指定一个颜色输出

void main()
{
	gl_Position = vec4(position, 1.0); // 注意我们如何把一个 vec3 作为 vec4 的构造器的参数
	vertextColor = vec4(0.5f, 0.0f, 0.0f, 1.0f);
}
