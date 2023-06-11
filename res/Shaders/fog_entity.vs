#version 330 core
// 迷雾的遮罩, 渲染到一整块迷雾贴图上

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
uniform mat4 projection;
uniform mat4 model;

void main()
{
	TexCoords = aTexCoords;
	gl_Position = projection * model * vec4(aPos, 1.0);
}
