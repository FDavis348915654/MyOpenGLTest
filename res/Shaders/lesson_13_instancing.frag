#version 330 core
// 实例化

out vec4 FragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

in vec3 fColor;

void main()
{
	FragColor = vec4(fColor, 1.0);
}
