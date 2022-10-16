#version 330 core
// 法线贴图

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

out VS_OUT {
	vec3 FragPos;
	vec2 TexCoords;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.TexCoords = aTexCoords;
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}
