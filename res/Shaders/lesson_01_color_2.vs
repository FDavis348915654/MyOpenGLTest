#version 330 core

// 基础光照 // 练习题四, 尝试实现一个Gouraud着色（而不是冯氏着色）。如果你做对了话，立方体的光照应该会看起来有些奇怪，尝试推理为什么它会看起来这么奇怪

layout (location = 0) in vec3 aPos; // aPos 变量的属性位置值为 0
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform vec3 objectColor;
uniform vec3 lightColor;

out vec3 resultColor;

void main()
{
	// 注意乘法要从右向左读
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vec3 Normal = mat3(transpose(inverse(model))) * aNormal;
	vec3 FragPos = vec3(model * vec4(aPos, 1.0));
	
	// 环境光
	float ambientStrength = 0.1f; // 环境光强度
	vec3 ambient = ambientStrength * lightColor;
	// 漫反射
	float diffStrength = 1.0f;
	vec3 norm = normalize(aNormal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0) * diffStrength;
	vec3 diffuse = diff * lightColor;
	// 镜面反射
	float specularStrength = 0.5f; // 镜面强度
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32); // 反光度
	vec3 specular = specularStrength * spec * lightColor;
	
	resultColor = (ambient + diffuse + specular) * objectColor;
}
