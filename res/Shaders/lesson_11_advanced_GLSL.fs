#version 330 core
// 高级数据/高级GLSL

out vec4 FragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

// 接口块
in VS_OUT
{
	vec2 TexCoords;
} vs_in;

void main()
{
	FragColor = vec4(1.0, 0.5, 0.0, 1.0);
}
