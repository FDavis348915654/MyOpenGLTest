#version 330 core
// 模板测试

uniform vec3 outlineColor;

out vec4 FragColor; // 片段着色器输出的变量名可以任意命名，类型必须是 vec4

void main()
{
	// FragColor = vec4(0.04, 0.28, 0.26, 1.0);
	FragColor = vec4(outlineColor, 1.0);
}
