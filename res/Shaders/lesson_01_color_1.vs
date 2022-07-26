#version 330 core

// 基础光照 // 练习题三, 在观察空间（而不是世界空间）中计算冯氏光照

layout (location = 0) in vec3 aPos; // aPos 变量的属性位置值为 0
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 FragPos;
out vec3 VlightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;

void main()
{
	// 注意乘法要从右向左读
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	Normal = mat3(transpose(inverse(view * model))) * aNormal;
	FragPos = vec3(view * model * vec4(aPos, 1.0));
	VlightPos = vec3(view * vec4(lightPos, 1.0));
}
