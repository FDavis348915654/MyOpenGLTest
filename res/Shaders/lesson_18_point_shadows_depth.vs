#version 330 core
// 点光源阴影

layout (location = 0) in vec3 aPos;

uniform mat4 model;

void main()
{
	gl_Position = model * vec4(aPos, 1.0f);
}
