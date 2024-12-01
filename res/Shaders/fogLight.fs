#version 330 core
// 迷雾灯光

in vec2 TexCoords;

out vec4 FragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

// Material
uniform sampler2D texture_diffuse1; // 漫反射的表面颜色

// 用于标记哪些地方需要"挖洞", 实际上只用到了 fogLightValue.r 和 纹理的 alpha
vec3 fogLightValue = vec3(1.0, 0.0, 0.0);

void main()
{
	FragColor = vec4(fogLightValue, texture(texture_diffuse1, TexCoords).a);
}
