#version 330 core
// 抗锯齿

layout (location = 0) in vec3 aPos; // aPos 变量的属性位置值为 0

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}
