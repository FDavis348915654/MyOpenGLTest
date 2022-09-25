#version 330 core
// 阴影映射

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;

void main()
{
	gl_Position = vec4(position, 1.0f);
	TexCoords = texCoords;
}
