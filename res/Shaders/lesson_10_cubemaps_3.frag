#version 330 core
// 立方体贴图 // 折射
in vec3 Normal;
in vec3 Position;

out vec4 FragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

uniform samplerCube skybox;
uniform vec3 cameraPos;

void main()
{
	float ratio = 1.0 / 1.52;
	//float ratio = 0.98;
	//float ratio = 1.4;
	vec3 I = normalize(Position - cameraPos);
	vec3 R = refract(I, normalize(Normal), ratio);
	FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
