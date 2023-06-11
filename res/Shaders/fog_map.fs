#version 330 core
// 绘制整块迷雾

in vec2 TexCoords;
out vec4 FragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

uniform sampler2D diffuse; // 漫反射纹理

void main()
{
	vec4 result = texture(diffuse, TexCoords);
	result.a = 1.0 - result.a;
	FragColor = result;
}
