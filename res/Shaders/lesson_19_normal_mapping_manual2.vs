#version 330 core
// 法线贴图, 手工计算法线(在 vs 里使用 TBN)

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;


out vec2 TexCoords;

out VS_OUT {
	vec3 FragPos;
	vec2 TexCoords;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangetFragPos;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.TexCoords = aTexCoords;

	vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
	vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
	vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
	mat3 TBN = transpose(mat3(T, B, N));

	vs_out.TangentLightPos = TBN * lightPos;
	vs_out.TangentViewPos = TBN * viewPos;
	vs_out.TangetFragPos = TBN * vs_out.FragPos;

	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}
