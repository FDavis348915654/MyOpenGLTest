#version 330 core
// 几何着色器

layout (location = 0) in vec3 aPos; // aPos 变量的属性位置值为 0
layout (location = 1) in vec3 aNormal;

out VS_OUT {
	vec3 normal;
} vs_out;

uniform mat4 model;
uniform mat4 view;

void main()
{
	// 注意乘法要从右向左读
	gl_Position = view * model * vec4(aPos, 1.0);
	mat3 normalMatrix = mat3(transpose(inverse(view * model)));
	vs_out.normal = normalize(vec3(vec4(normalMatrix * aNormal, 0.0)));
}
