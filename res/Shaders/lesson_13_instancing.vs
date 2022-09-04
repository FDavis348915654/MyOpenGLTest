#version 330 core
// 实例化

layout (location = 0) in vec2 aPos; // aPos 变量的属性位置值为 0
layout (location = 1) in vec3 aColor;

out vec3 fColor;

uniform vec2 offsets[100];

void main()
{
	vec2 offset = offsets[gl_InstanceID];
	gl_Position = vec4(aPos + offset, 0.0, 1.0);
	fColor = aColor;
}
