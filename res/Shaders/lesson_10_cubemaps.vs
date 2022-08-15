#version 330 core
// 立方体贴图
layout (location = 0) in vec3 aPos; // aPos 变量的属性位置值为 0

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	TexCoords = aPos;
	gl_Position = projection * view * vec4(aPos, 1.0);
}
