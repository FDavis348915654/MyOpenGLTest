#version 330 core
// 实例化

layout (location = 0) in vec3 aPos; // aPos 变量的属性位置值为 0
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 lightPos;
uniform vec3 viewPos;

out VS_OUT {
	vec3 FragPos;
	vec2 TexCoords;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangetFragPos;
} vs_out;

void main()
{
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.TexCoords = aTexCoords;

	// 修正前
	// vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
	// vec3 B = normalize(vec3(model * vec4(aBitangent, 0.0)));
	// vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));

	// 修正后
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vec3 T = normalize(normalMatrix * aTangent);
	vec3 N = normalize(normalMatrix * aNormal);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	mat3 TBN = transpose(mat3(T, B, N));
	vs_out.TangentLightPos = TBN * lightPos;
	vs_out.TangentViewPos = TBN * viewPos;
	//vs_out.TangetFragPos = TBN * vec3(model * vec4(aPos, 0.0)); // 这个写法是错的, 教程里写错了, 应该是 1.0
	vs_out.TangetFragPos = TBN * vs_out.FragPos;

	// 注意乘法要从右向左读
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}
