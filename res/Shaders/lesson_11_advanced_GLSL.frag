#version 330 core
// 高级数据/高级GLSL

out vec4 FragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

// Material
uniform sampler2D texture_diffuse1; // 漫反射的表面颜色

void main()
{
	FragColor = vec4(1.0, 0.5, 0.0, 1.0);
}
