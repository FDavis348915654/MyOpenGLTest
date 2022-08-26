#version 330 core
// 高级数据/高级GLSL // Uniform缓冲对象

layout (location = 0) in vec3 aPos; // aPos 变量的属性位置值为 0

layout (std140) uniform Matrices
{
	uniform mat4 projection;
	uniform mat4 view;
};

uniform mat4 model;

void main()
{
	// 注意乘法要从右向左读
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}
