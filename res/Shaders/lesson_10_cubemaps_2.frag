#version 330 core
// 立方体贴图 // 反射
in vec3 Normal;
in vec3 Position;

out vec4 FragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

uniform samplerCube skybox;
uniform vec3 cameraPos;

void main()
{
	vec3 I = normalize(Position - cameraPos);
	vec3 R = reflect(I, normalize(Normal));
	FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
