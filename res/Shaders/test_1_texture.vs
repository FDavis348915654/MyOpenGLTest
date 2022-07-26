#version 330 core

layout (location = 0) in vec3 aPos; // aPos 变量的属性位置值为 0
layout (location = 1) in vec3 aColor; // aColor 变量的属性位置值为 1
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 texCoord;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	ourColor = aColor;
	texCoord = aTexCoord;
}
