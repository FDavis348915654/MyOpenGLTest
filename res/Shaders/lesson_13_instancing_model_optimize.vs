#version 330 core
// 实例化

layout (location = 0) in vec3 aPos; // aPos 变量的属性位置值为 0
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 instanceMatrix;

out vec2 TexCoords;

uniform mat4 view;
uniform mat4 projection;

void main()
{
	// 注意乘法要从右向左读
	gl_Position = projection * view * instanceMatrix * vec4(aPos, 1.0);
	TexCoords = aTexCoords;
}